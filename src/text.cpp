#include "text.h"
#include "GLFW/glfw3.h"

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
    : TextShader("Resources/shaders/text.vert", "Resources/shaders/text.frag") {
  // load and configure shader
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f,
                                    static_cast<float>(height));
  TextShader.use();
  glUniformMatrix4fv(glGetUniformLocation(TextShader.ID, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));
  // configure VAO/VBO for texture quads
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, unsigned int fontSize) {

  // OpenGL state
  // ------------
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // first clear the previously loaded Characters
  this->Characters.clear();
  // then initialize and load the FreeType library
  if (FT_Init_FreeType(&ft)) { // all functions return a value different than 0
                               // whenever an error occurred
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
  }

  if (font.empty()) {
    std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
  }

  // load font as face
  FT_Face face;
  if (FT_New_Face(ft, font.c_str(), 0, &face)) {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
  }
  // set size to load glyphs as
  // Check for the correct way to size your fonts
  if (FT_HAS_FIXED_SIZES(face)) {
    FT_Select_Size(face, 0); // just use the first one
  } else {
    FT_Set_Pixel_Sizes(face, 0, fontSize);
  }
  // disable byte-alignment restriction
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Assign the appropriate load flags
  int loadFlag;
  if (FT_HAS_COLOR(face)) {
    loadFlag = FT_LOAD_COLOR | FT_LOAD_DEFAULT;
  } else {
    loadFlag = FT_LOAD_DEFAULT;
  }

  FT_Select_Charmap(face, FT_ENCODING_UNICODE);

  // then for the first 255 ASCII characters, pre-load/compile their characters
  // and store them
  for (unsigned char c = 32; c < 255; c++) {
    // load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if ((loadFlag & FT_LOAD_COLOR) == 1) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, face->glyph->bitmap.width,
                   face->glyph->bitmap.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                   face->glyph->bitmap.buffer);
    } else {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                   face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                   face->glyph->bitmap.buffer);
    }

    // now store character for later use
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
    Characters.insert(std::pair<char, Character>(c, character));
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  // destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale,
                              glm::vec3 color) {
  // activate corresponding render state
  this->TextShader.use();
  glUniform3f(glGetUniformLocation(TextShader.ID, "textColor"), color.x,
              color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(this->VAO);
  float xpos = x;
  float cursorVertices[6][2] = {
      {10.8 + 3, 550 + 30}, {10.8 + 3, 550}, {10.8 + 5, 550},

      {10.8 + 3, 550 + 30}, {10.8 + 5, 550}, {10.8 + 5, 550 + 30}};

  // iterate through all characters
  std::string::const_iterator c;
  int var = 1;
  float cursorXpos = Characters['I'].Size.x - Characters['I'].Bearing.x;
  float cursorYpos = Characters['C'].Size.y - Characters['C'].Bearing.y;
  bool cursorDraw = false;
  if (textString.size() == 0) {
    cursor = textString.size();
    cursorMinus = 0;
  } else {
    cursor =
        textString.size() - cursorMinus; // Position of the cursor, cursorMinus
                                         // is a var to help locate the place
  }
  if (cursor < 0) {
    cursor = 0;
  }
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = Characters[*c];
    if (*c == '\n') {
      // Initializing x to its base value so it doesnt have that offset
      x = xpos;
      y -= (Characters['C'].Size.y + Characters['C'].Size.r) *
           scale; // The amount of unit the text goes down
    }

    if (var == cursor) {
      cursorXpos = (x + (ch.Advance >> 6) * scale) + ch.Bearing.x - 5 * scale;
      cursorYpos =
          y - (Characters['C'].Size.y - Characters['C'].Bearing.y) * scale;
      cursorDraw = true;
    }

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
    bool newLine = false;
    // Auto New Line
    if (xpos > 770.0f) {
      int wordSize = 30;
      for (size_t i = text.size(); i > 0 && i < text.size() + wordSize;
           i--) { // Make the whole word go to a new line
        if (text[i] == ' ') {
          textString[i] = '\n';
          newLine = true;
          break;
        }
      }
      if (!newLine) { // Make only the char that is out of bound into a new line
        textString.insert(var - 1, "\n");
        break;
      }
    }

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    // update VBO for each character
    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},

        {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}};
    if (cursorDraw) {
      float cursorPoints[6][2] = {
          // 3 is how much i want it to be next to the last char and 5 is the
          // width of the rectangle and 40 is the height
          {cursorXpos + 3, cursorYpos + 40}, {cursorXpos + 3, cursorYpos},
          {cursorXpos + 5, cursorYpos},

          {cursorXpos + 3, cursorYpos + 40}, {cursorXpos + 5, cursorYpos},
          {cursorXpos + 5, cursorYpos + 40}};
      memcpy(cursorVertices, cursorPoints,
             sizeof(cursorVertices)); // array = to another array so i can pass
                                      // the data outside the loop
    }
    // Remove the transparent settings
    glUniform3f(glGetUniformLocation(TextShader.ID, "textColor"), color.x,
                color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBufferSubData(
        GL_ARRAY_BUFFER, 0, sizeof(vertices),
        vertices); // be sure to use glBufferSubData and not glBufferData
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // now advance cursors for next glyph
    x += (ch.Advance >> 6) *
         scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    var++;
  }
  if (textMode) { // Render Caret
    // Remove the transparent settings
    glUniform3f(glGetUniformLocation(TextShader.ID, "textColor"), 1.0, 1.0,
                1.0);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ZERO);
    // Remove the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cursorVertices), cursorVertices,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
