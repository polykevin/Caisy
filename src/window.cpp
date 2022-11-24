#include "window.h"
#include "glm/fwd.hpp"
#include <string>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.0f;
float lastFrame = 0.0f;
float deltaTime = 0.0f;
bool textMode = false;
double xposition, yposition;
bool press = false;
std::string textString = "";
std::vector<std::string> history_Z;
int nbReturn_Z = 0;
std::vector<std::string> history_Y;
int nbReturn_Y = 0;
int cursor = 0;
int cursorMinus = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    textMode = false;
  }

  if (textMode) {
    // Erase
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_REPEAT ||
        key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
      if (textString.length() > 0) {
        history_Z.push_back(textString);
        if (textString.size() - cursorMinus > 0 && cursor >= 0) {
          textString.erase(textString.end() - (cursorMinus + 1));
        }

        cursor = textString.size() - cursorMinus; // Update the cursor
      }
    }
    // New line
    if (key == GLFW_KEY_ENTER && action == GLFW_REPEAT ||
        key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
      history_Z.push_back(textString);
      if (textString.size() == 0) {
        cursor = textString.size();
        cursorMinus = 0;
      } else {
        cursor = textString.size() -
                 cursorMinus; // Position of the cursor, cursorMinus is a var to
                              // help locate the place
      }

      if (cursor == textString.size()) {
        textString += '\n';
      } else {
        textString.insert(cursor, "\n");
      }
    }

    // Erase whole word "CTRL + ENTER"
    int stateLCtrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    int stateBs = glfwGetKey(window, GLFW_KEY_BACKSPACE);
    if (stateLCtrl == GLFW_PRESS && stateBs == GLFW_PRESS ||
        stateLCtrl == GLFW_REPEAT && stateBs == GLFW_REPEAT) {
      int erase = 0;
      if (textString.length() > 0 && cursor > 0) {
        for (size_t i = textString.size() - 1; i > 0; i--) { // Find the Word
          if (textString[i] == ' ') {
            history_Z.push_back(textString); // Ctrl Z history
            textString.erase(
                i, (erase - cursorMinus + 1)); // Erase the char to the space
            break;
          }
          if (i == 1) {
            history_Z.push_back(textString);
            textString.erase(textString.begin(),
                             textString.end() -
                                 cursorMinus); // Erase the whole thing
            break;
          }
          erase++;
        }
      }
      cursor = textString.size() - cursorMinus; // Update the cursor
    }

    // CTRL + Z
    int stateZ = glfwGetKey(window, GLFW_KEY_W);
    if (stateLCtrl == GLFW_PRESS && stateZ == GLFW_PRESS ||
        stateLCtrl == GLFW_REPEAT && stateZ == GLFW_REPEAT) {
      history_Y.push_back(textString);
      nbReturn_Z += 1;
      int row = history_Z.size() - (1 * nbReturn_Z);
      if (textString != "") {
        if (row >= 0) {
          textString = history_Z.at(row);
        }
      } else {
        nbReturn_Z = 0;
        history_Z.clear();
      }
    }
    // CTRL + Y
    int stateY = glfwGetKey(window, GLFW_KEY_Y);
    if (stateLCtrl == GLFW_PRESS && stateY == GLFW_PRESS ||
        stateLCtrl == GLFW_REPEAT && stateY == GLFW_REPEAT) {
      nbReturn_Y += 1;
      int row = history_Y.size() - (1 * nbReturn_Y);
      if (row >= 0 && textString != "") {
        history_Z.push_back(textString);
        textString = history_Y.at(row);
      } else {
        nbReturn_Y = 0;
        history_Y.clear();
      }
    }
    // Ctrl + V
    int stateV = glfwGetKey(window, GLFW_KEY_V);
    if (stateLCtrl == GLFW_PRESS && stateV == GLFW_PRESS ||
        stateLCtrl == GLFW_REPEAT && stateV == GLFW_REPEAT) {
      const char *clipBoard = glfwGetClipboardString(window);
      history_Z.push_back(textString);
      textString += clipBoard;
    }
    // Left Arrow
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT ||
        key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
      if (cursorMinus < textString.size()) {
        cursorMinus++;
      }
    }
    // Right Arrow
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT ||
        key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
      if (cursorMinus > 0) {
        cursorMinus--;
      }
    }
    // Down Arrow
    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT ||
        key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
      int nPos_FirstRow = 0;
      int nPos_SecondRow = 0;
      bool blocked = false;
      int count = 0;
      int index = 0;
      cursor = textString.size() - cursorMinus;

      for (int i = cursor; i >= 0 && i <= textString.size(); i++) {
        if (textString[i] == '\n') {
          nPos_FirstRow = i + 1;
          break;
        }
        if (i >= textString.size()) { // verify if you can go down
          blocked = true;
        }
      }

      if (!blocked) {
        // Find the number of letter (first row)
        for (int i = cursor - 1; i >= 0 && i <= textString.size(); i--) {
          if (textString[i] == '\n') {
            break;
          }
          count++;
        }
        // 2nd row
        for (int i = nPos_FirstRow; i < textString.size(); i++) {
          nPos_SecondRow = 1 + i;
          if (textString[i] == '\n') {
            nPos_SecondRow = i;
            break;
          }
          index++;
        }

        if (count <= index) {
          nPos_SecondRow = nPos_FirstRow + count;
        }

        cursor = nPos_SecondRow;
        cursorMinus = textString.size() - cursor;
      }
    }
  }
  // Up Arrow
  if (key == GLFW_KEY_UP && action == GLFW_REPEAT ||
      key == GLFW_KEY_UP && action == GLFW_PRESS) {
    int nPos_FirstRow = 0;
    int nPos_SecondRow = 0;
    bool blocked = false;
    int count = 0;
    int index = 0;
    cursor = textString.size() - cursorMinus;

    for (int i = cursor - 1; i >= 0 && i <= textString.size(); i--) {
      if (textString[i] == '\n') {
        nPos_FirstRow = i - 1;
        break;
      }
      if (i <= 0) { // verify if you can go up
        blocked = true;
      }
    }

    if (!blocked) {
      // Find the number of letter (first row)
      for (int i = cursor - 1; i >= 0 && i <= textString.size(); i--) {
        if (textString[i] == '\n') {
          break;
        }
        count++;
      }
      // 2nd row
      for (int i = nPos_FirstRow; i >= 0 && i < textString.size(); i--) {
        if (textString[i] == '\n') {
          break;
        }
        nPos_SecondRow = i;
        index++;
      }

      if (count <= index) {
        nPos_SecondRow += count;
      } else {
        nPos_SecondRow = nPos_FirstRow + 1;
      }

      cursor = nPos_SecondRow;
      cursorMinus = textString.size() - cursor;
    }
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

    glfwGetCursorPos(window, &xposition, &yposition);

    textMode = true;
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  cameraSpeed = 2800.0f * deltaTime;
  if (yoffset > 0) {
    cameraPos += (float)yoffset * cameraSpeed * cameraUp;
  }
  if (yoffset < 0) {
    cameraPos -= (float)yoffset * -1 * cameraSpeed * cameraUp;
  }
}

void character_callback(GLFWwindow *window, uint32_t codepoint) {
  const uint32_t test = codepoint;
  std::cout << test << "\n";
  setlocale(LC_ALL, "");
  if (textMode) {
    if (textString.size() == 0) {
      cursor = textString.size();
      cursorMinus = 0;
    } else {
      cursor = textString.size() -
               cursorMinus; // Position of the cursor, cursorMinus is a var to
                            // help locate the place
    }
    if (cursor < 0) {
      cursor = 0;
    }

    if (cursor == textString.size()) {
      history_Z.push_back(textString); // Update the history
      textString += codepoint; // Updating the text with the new character
    } else {
      std::string placeHolder = "";
      placeHolder += codepoint;

      history_Z.push_back(textString);        // Update the history
      textString.insert(cursor, placeHolder); // Update the text with the new
                                              // character (inside the text)
    }
  }
}
