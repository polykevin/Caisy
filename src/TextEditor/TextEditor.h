/*Copyright 2022 Kevin Poly*/
#pragma once
#include "../Renderer/EBO/EBO.h"
#include "../Renderer/Shader/Shader.h"
#include "../Renderer/VAO/VAO.h"
#include "../Renderer/VBO/VBO.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../App/Object.h"
#include "../App/WindowClass.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/Event.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <pango.h>
#include <string>
#include <unicode/uconfig.h>
#include <unicode/unistr.h>
#include <unordered_map>
#include <vector>

namespace TextEditor {
class TextRenderer : public Object {
public:
  TextRenderer();
  ~TextRenderer();

  void Run();
  void Load();

  void LoadFile(const std::string &filepath);

  void OnEvent(Event &e);

private:
  bool OnKey(KeyPressedEvent &e);
  bool OnScroll(MouseScrolledEvent &e);
  bool OnChar(KeyTypedEvent &e);
  bool OnMouse(MouseButtonPressedEvent &e);
  bool OnResize(WindowResizeEvent &e);

  void RenderText(glm::vec3 color);
  void ComputeBitmap();
  void CameraFollow();

  Shader m_TextShader;
  Shader m_BasicShader;
  VAO_Class m_VAO;
  VBO_Class m_VBO;
  VBO_Class m_CaretVbo;
  icu::UnicodeString m_Text;
  std::string m_FilePath;

  PangoContext *m_Context = NULL;
  PangoLayout *m_Layout;
  PangoFontDescription *m_FontDesc = NULL;
  PangoFontMap *m_FontMap = NULL;
  PangoRectangle m_CaretRect;
  unsigned int *m_Textures;
  unsigned int m_Texture;
  FT_Bitmap m_Bitmap;
  int m_ControlHeight = m_Height;
  int m_LayoutWidth = m_Width;
  int m_LayoutHeight = m_Height;

  int m_Cursor = 0;
  int m_CursorMinus = 0;

  int m_Width = 1280;
  int m_Height = 720;

  bool m_TextMode = false;

  glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
  glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  float m_CameraSpeed = 0.0f;

  float m_DeltaTime = 0.0f;
  float m_LastFrame = 0.0f;
};
} // namespace TextEditor
