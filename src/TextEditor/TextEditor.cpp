/*Copyright 2022 Kevin Poly*/
#include "TextEditor.h"
#include "../App/Application.h"
#include "../App/WindowClass.h"
#include "../Events/Input.h"
#include "freetype/ftimage.h"
#include "pango/pango-layout.h"
#include "pango/pango-types.h"
#include <GLFW/glfw3.h>
#include <freetype/ftbitmap.h>
#include <ft2build.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ostream>
#include <pango/pangoft2.h>
#include <sstream>
#include <string>
#include <unicode/uconfig.h>
#include <unicode/unistr.h>
#include <vector>
#include FT_FREETYPE_H

namespace TextEditor {

#define BIND_EVENT_FNC(x) std::bind(&x, this, std::placeholders::_1)

TextRenderer::TextRenderer()
    : m_TextShader("Resources/Shaders/text.shader"),
      m_BasicShader("Resources/Shaders/basic.shader"), m_VBO(), m_CaretVbo() {
  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));

  m_BasicShader.Bind();
  m_BasicShader.SetMat4("projection", 1, GL_FALSE, projection);

  m_TextShader.Bind();
  m_TextShader.SetMat4("projection", 1, GL_FALSE, projection);

  m_VAO.Bind();

  m_VBO.Bind();
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr,
               GL_DYNAMIC_DRAW);
  VertexArrayLayout layout;
  layout.PushFloat(4);
  m_VAO.AddBuffer(m_VBO, layout);

  m_CaretVbo.Bind();
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, nullptr,
               GL_DYNAMIC_DRAW);
  layout.PushFloat(2);
  m_VAO.AddBuffer(m_CaretVbo, layout);

  unsigned int *m_Textures = nullptr;
  unsigned int m_Texture = -1;

  Load();
}

TextRenderer::~TextRenderer() {
  delete[] m_Bitmap.buffer;
  g_object_unref(m_Layout);
  g_object_unref(m_FontMap);
  g_object_unref(m_Context);
}

void TextRenderer::LoadFile(const std::string &filepath) {
  std::stringstream ss;
  std::string line;
  std::ifstream stream{filepath};

  if (!stream)
    return;

  while (std::getline(stream, line)) {
    ss << line << '\n';
  }
  stream.close();

  const std::string textFile = ss.str();

  m_Text = icu::UnicodeString::fromUTF8(icu::StringPiece(textFile.c_str()));
  ComputeBitmap(); // Change the texture
}

void TextRenderer::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FNC(TextRenderer::OnKey));
  dispatcher.Dispatch<MouseScrolledEvent>(
      BIND_EVENT_FNC(TextRenderer::OnScroll));
  dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FNC(TextRenderer::OnChar));
  dispatcher.Dispatch<MouseButtonPressedEvent>(
      BIND_EVENT_FNC(TextRenderer::OnMouse));
}

bool TextRenderer::OnChar(KeyTypedEvent &e) {
  if (m_TextMode) {
    if (m_Text.length() == 0) {
      m_Cursor = m_Text.length();
      m_CursorMinus = 0;
    } else {
      m_Cursor = m_Text.length() -
                 m_CursorMinus; // Position of the cursor, cursorMinus is a var
                                // to help locate the place
    }
    if (m_Cursor < 0) {
      m_Cursor = 0;
    }

    if (m_Cursor == m_Text.length()) {
      m_Text.append(
          (UChar32)e.GetKeyCode()); // Updating the text with the new character
    } else {

      m_Text.insert(m_Cursor,
                    (UChar32)e.GetKeyCode()); // Update the text with the new
                                              // character (inside the text)
    }
    ComputeBitmap(); // Change the texture
  }
  return false;
}

bool TextRenderer::OnScroll(MouseScrolledEvent &e) {
  m_CameraSpeed = 2800.0f * m_DeltaTime;
  glm::vec3 maxPos = glm::vec3(0.0f, 0.0f, 1.0f);
  bool top = (m_CameraPos.y > maxPos.y) ? true : false;

  if (e.GetYOffset() > 0 && !top) {
    m_CameraPos += (float)e.GetYOffset() * m_CameraSpeed * m_CameraUp;
  }
  if (e.GetYOffset() < 0) {
    m_CameraPos -= (float)e.GetYOffset() * -1 * m_CameraSpeed * m_CameraUp;
  }

  return false;
}

bool TextRenderer::OnMouse(MouseButtonPressedEvent &e) {
  if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT) {
    m_TextMode = true;
  }
  return false;
}

bool TextRenderer::OnKey(KeyPressedEvent &e) {
  int key = e.GetKeyCode();

  if (key == GLFW_KEY_ESCAPE) {
    m_TextMode = false;
  }

  if (m_TextMode) {
    // Erase
    if (key == GLFW_KEY_BACKSPACE) {
      if (m_Text.length() > 0) {
        if (m_Text.length() - m_CursorMinus > 0 && m_Cursor >= 0) {
          m_Text.remove(m_Cursor - 1, 1);
        }
        m_Cursor = m_Text.length() - m_CursorMinus;
      }
      ComputeBitmap(); // Change the texture
    }

    // New Line
    if (key == GLFW_KEY_ENTER) {
      if (m_Text.length() == 0) {
        m_Cursor = m_Text.length();
        m_CursorMinus = 0;
      } else {
        m_Cursor =
            m_Text.length() -
            m_CursorMinus; // CursorMinus is a var to help locate the place
      }

      if (m_Cursor == m_Text.length()) {
        m_Text += '\n';
      } else {
        m_Text.insert(m_Cursor, "\n");
      }
      ComputeBitmap(); // Change the texture
    }

    // Erase whole word "CTRL + BACKSPACE"
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) &&
        key == GLFW_KEY_BACKSPACE) {
      int erase = 0;
      if (m_Text.length() > 0 && m_Cursor > 0) {
        for (size_t i = m_Text.length() - 1; i > 0; i--) { // Find the Word
          if (m_Text[i] == ' ') {
            m_Text.remove(
                i, (erase - m_CursorMinus + 1)); // Erase the char to the space
            break;
          }
          if (i == 1) {
            m_Text.remove(0, m_Text.length() -
                                 m_CursorMinus); // Erase the whole thing
            break;
          }
          erase++;
        }
      }
      m_Cursor = m_Text.length() - m_CursorMinus; // Update the cursor
      ComputeBitmap();                            // Change the texture
    }

    // CTRL + Z
    /* if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && key == GLFW_KEY_W) { */
    /*     m_History_Y.push_back(m_Text); */
    /*     m_NbReturn_Z += 1; */
    /*     int row = m_History_Z.size() - (1 * m_NbReturn_Z); */
    /*     if (m_Text != "") { */
    /*         if (row >= 0) { */
    /*             m_Text = m_History_Z.at(row); */
    /*         } */
    /*     } else { */
    /*         m_NbReturn_Z = 0; */
    /*         m_History_Z.clear(); */
    /*     } */
    /*     ComputeBitmap(); // Change the texture */
    /* } */

    /* // CTRL + Y */
    /* if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && key == GLFW_KEY_Y) { */
    /*     m_NbReturn_Y += 1; */
    /*     int row = m_History_Y.size() - (1 * m_NbReturn_Y); */
    /*     if (row >= 0 && m_Text != "") { */
    /*         m_History_Z.push_back(m_Text); */
    /*         m_Text = m_History_Y.at(row); */
    /*     } else { */
    /*         m_NbReturn_Y = 0; */
    /*         m_History_Y.clear(); */
    /*     } */
    /*     ComputeBitmap(); // Change the texture */
    /* } */

    // Ctrl + V
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && key == GLFW_KEY_V) {
      const char *clipBoard = glfwGetClipboardString(static_cast<GLFWwindow *>(
          Application::Get()->GetWindow().GetNativeWindow()));
      m_Text.append(icu::UnicodeString::fromUTF8(icu::StringPiece(clipBoard)));
      ComputeBitmap(); // Change the texture
    }

    // Left Arrow
    if (key == GLFW_KEY_LEFT) {
      if (m_CursorMinus < m_Text.length()) {
        m_CursorMinus++;
      }
    }
    // Right Arrow
    if (key == GLFW_KEY_RIGHT) {
      if (m_CursorMinus > 0) {
        m_CursorMinus--;
      }
    }

    // Down Arrow
    if (key == GLFW_KEY_DOWN) {
      int nPos_FirstRow = 0;
      int nPos_SecondRow = 0;
      bool blocked = false;
      int count = 0;
      int index = 0;
      m_Cursor = m_Text.length() - m_CursorMinus;

      for (int i = m_Cursor; i >= 0 && i <= m_Text.length(); i++) {
        if (m_Text[i] == '\n') {
          nPos_FirstRow = i + 1;
          break;
        }
        if (i >= m_Text.length()) { // verify if you can go down
          blocked = true;
        }
      }

      if (!blocked) {
        // Find the number of letter (first row)
        for (int i = m_Cursor - 1; i >= 0 && i <= m_Text.length(); i--) {
          if (m_Text[i] == '\n') {
            break;
          }
          count++;
        }
        // 2nd row
        for (int i = nPos_FirstRow; i < m_Text.length(); i++) {
          nPos_SecondRow = 1 + i;
          if (m_Text[i] == '\n') {
            nPos_SecondRow = i;
            break;
          }
          index++;
        }

        if (count <= index) {
          nPos_SecondRow = nPos_FirstRow + count;
        }

        m_Cursor = nPos_SecondRow;
        m_CursorMinus = m_Text.length() - m_Cursor;
      }
    }

    // Up Arrow
    if (key == GLFW_KEY_UP) {
      int nPos_FirstRow = 0;
      int nPos_SecondRow = 0;
      bool blocked = false;
      int count = 0;
      int index = 0;
      m_Cursor = m_Text.length() - m_CursorMinus;

      for (int i = m_Cursor - 1; i >= 0 && i <= m_Text.length(); i--) {
        if (m_Text[i] == '\n') {
          nPos_FirstRow = i - 1;
          break;
        }
        if (i <= 0) { // verify if you can go up
          blocked = true;
        }
      }

      if (!blocked) {
        // Find the number of letter (first row)
        for (int i = m_Cursor - 1; i >= 0 && i <= m_Text.length(); i--) {
          if (m_Text[i] == '\n') {
            break;
          }
          count++;
        }
        // 2nd row
        for (int i = nPos_FirstRow; i >= 0 && i < m_Text.length(); i--) {
          if (m_Text[i] == '\n') {
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

        m_Cursor = nPos_SecondRow;
        m_CursorMinus = m_Text.length() - m_Cursor;
      }
    }
  }

  return false;
}

static float bottom = 0;
static int controlHeight = 720;
static int width = 1280;
static int height = 720;
static int texLimit = 0;

void TextRenderer::ComputeBitmap() {
  std::string s;
  m_Text.toUTF8String(s);
  pango_layout_set_text(m_Layout, s.c_str(), -1);
  pango_layout_context_changed(m_Layout);
  pango_layout_get_pixel_size(m_Layout, &width, &height);

  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texLimit);

  delete[] m_Bitmap.buffer;

  m_Bitmap.rows = height;
  m_Bitmap.width = width;
  m_Bitmap.pitch = (m_Bitmap.width + 3) & ~3;
  m_Bitmap.buffer = new unsigned char[m_Bitmap.pitch * m_Bitmap.rows];
  m_Bitmap.num_grays = 256;
  m_Bitmap.pixel_mode = FT_PIXEL_MODE_GRAY;

  memset(m_Bitmap.buffer, 0x00, m_Bitmap.pitch * m_Bitmap.rows);
  pango_ft2_render_layout(&m_Bitmap, m_Layout, 0, 0);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Bitmap.pitch, m_Bitmap.rows, 0,
               GL_RED, GL_UNSIGNED_BYTE, m_Bitmap.buffer);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (controlHeight != height) {
    controlHeight = height;
    bottom = (float)height;
  }
}

void TextRenderer::Load() {
  FT_Bitmap_Init(&m_Bitmap);

  m_FontMap = pango_ft2_font_map_new();
  pango_ft2_font_map_set_resolution(PANGO_FT2_FONT_MAP(m_FontMap), 96, 96);
  m_Context = pango_font_map_create_context(m_FontMap);
  m_Layout = NULL;
  m_Layout = pango_layout_new(m_Context);
  pango_context_set_round_glyph_positions(m_Context, FALSE);

  m_FontDesc = pango_font_description_from_string("Arial Bold 50");
  pango_layout_set_font_description(m_Layout, m_FontDesc);
  pango_font_map_load_font(m_FontMap, m_Context, m_FontDesc);
  pango_font_description_free(m_FontDesc);

  pango_layout_set_width(m_Layout, 1280 * PANGO_SCALE);
  pango_layout_set_wrap(m_Layout, PANGO_WRAP_WORD_CHAR);

  glGenTextures(1, &m_Texture);
}

// TO DO : Cursor Caret Rendering & Slice the text to many textures
void TextRenderer::RenderText(glm::vec3 color) {
  m_TextShader.Bind();
  m_TextShader.Set3F("textColor", color);

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float vertices[] = {
      // vec2 Pos ; Vec2 Tex
      0.0f,         720.0f,
      0.0f,         0.0f,
      0.0f,         (720.0f - bottom),
      0.0f,         1.0f,
      (float)width, (720.0f - bottom),
      1.0f,         1.0f,

      0.0f,         720.0f,
      0.0f,         0.0f,
      (float)width, (720.0f - bottom),
      1.0f,         1.0f,
      (float)width, 720.0f,
      1.0f,         0.0f,
  };

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Texture);
  m_VAO.Bind();
  m_VBO.Bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  // render quad
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Render Caret
  if (m_TextMode) {
    PangoLayoutIter *test = pango_layout_get_iter(m_Layout);

    if (m_Text.length() == 0) {
      m_Cursor = m_Text.length();
      m_CursorMinus = 0;
    } else {
      m_Cursor = m_Text.length() -
                 m_CursorMinus; // Position of the cursor, cursorMinus is a var
                                // to help locate the place
    }
    if (m_Cursor < 0) {
      m_Cursor = 0;
    }

    for (int i = 0; i < m_Cursor; i++) {
      pango_layout_iter_next_char(test);
    }
    pango_layout_get_caret_pos(m_Layout, pango_layout_iter_get_index(test),
                               &m_CaretRect, NULL);
    pango_extents_to_pixels(&m_CaretRect, NULL);

    m_CaretRect.width += 3.0f;

    float cursorVertices[] = {
        (float)m_CaretRect.x,
        (float)(720.0f - m_CaretRect.y),
        (float)m_CaretRect.x,
        (float)(720.0f - m_CaretRect.height - m_CaretRect.y),
        (float)(m_CaretRect.width + m_CaretRect.x),
        (float)(720.f - m_CaretRect.height - m_CaretRect.y),

        (float)m_CaretRect.x,
        (float)(720.0f - m_CaretRect.y),
        (float)(m_CaretRect.width + m_CaretRect.x),
        (float)(720.0f - m_CaretRect.y),
        (float)(m_CaretRect.width + m_CaretRect.x),
        (float)(720.0f - m_CaretRect.height - m_CaretRect.y),
    };

    glDisable(GL_CULL_FACE);
    // Remove Texture
    glBindTexture(GL_TEXTURE_2D, 0);
    m_BasicShader.Bind();
    m_VAO.Bind();
    m_CaretVbo.Bind();
    m_BasicShader.Set3F("Color", color);
    m_BasicShader.Set1F("Time", glfwGetTime());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cursorVertices), cursorVertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_BasicShader.Unbind();
  }

  m_VBO.Unbind();
  m_CaretVbo.Unbind();
  m_VAO.Unbind();
}

void TextRenderer::Run() {
  float currentFrame = glfwGetTime();
  m_DeltaTime = currentFrame - m_LastFrame;
  m_LastFrame = currentFrame;
  // Shows the text
  glDepthMask(GL_FALSE);
  RenderText(glm::vec3(0.913f, 0.392f, 0.474f));
  glDepthMask(GL_TRUE);
  glm::mat4 view = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront,
                               m_CameraUp); // move the camera
  m_TextShader.Bind();
  m_TextShader.SetMat4("view", 1, GL_FALSE, view);

  m_BasicShader.Bind();
  m_BasicShader.SetMat4("view", 1, GL_FALSE, view);
}
} // namespace TextEditor
