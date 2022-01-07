#include "stdafx.h"
#include "EImmediateUI.h"

#ifdef PLATFORM_WINDOWS
//Statics
float Elite::EImmediateUI::m_sMouseWheel = 0.0f;
bool Elite::EImmediateUI::m_sMousePressed[3] = { false, false, false };
GLuint Elite::EImmediateUI::m_programID = 0;
GLuint Elite::EImmediateUI::m_vboID = 0, Elite::EImmediateUI::m_vaoID = 0, Elite::EImmediateUI::m_elementsID = 0;
GLint Elite::EImmediateUI::m_textureUniform = 0, Elite::EImmediateUI::m_projectionUniform = 0;
GLint Elite::EImmediateUI::m_positionAttribute = 0, Elite::EImmediateUI::m_uvAttribute = 0, Elite::EImmediateUI::m_colorAttribute = 0;

Elite::EImmediateUI::~EImmediateUI()
{
	//Shutdown
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_elementsID);
	glDeleteProgram(m_programID);
	glDeleteTextures(1, &m_atlasTextureID);
	ImGui::GetIO().Fonts->TexID = nullptr;
	m_atlasTextureID = 0;
	ImGui::Shutdown();
}

void Elite::EImmediateUI::Initialize(EliteRawWindow pWindow)
{
	//Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = SDLK_a;
	io.KeyMap[ImGuiKey_C] = SDLK_c;
	io.KeyMap[ImGuiKey_V] = SDLK_v;
	io.KeyMap[ImGuiKey_X] = SDLK_x;
	io.KeyMap[ImGuiKey_Y] = SDLK_y;
	io.KeyMap[ImGuiKey_Z] = SDLK_z;

	//Set function pointers
	io.RenderDrawListsFn = &EImmediateUI::StaticRender;
	io.SetClipboardTextFn = &EImmediateUI::SetClipboardText;
	io.GetClipboardTextFn = &EImmediateUI::GetClipboardText;

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	auto succeeded = SDL_GetWindowWMInfo(pWindow, &wmInfo);
	if (!succeeded)
		return;
	io.ImeWindowHandle = wmInfo.info.win.window;

	//Create shaderprogram using the Renderer function
	m_programID = DEBUGRENDERER2D->LoadShadersToProgramFromEmbeddedSource(ImGuiVertexShaderSource, ImGuiFragmentShaderSource);
	/*m_programID = DEBUGRENDERER2D->LoadShadersToProgram("../data/shaders/ImGUIVertexShader.vertexshader",
		"../data/shaders/ImGUIFragmentShader.fragmentshader");*/
	//Create buffers and link uniforms & attributes
	m_textureUniform = glGetUniformLocation(m_programID, "_texture");
	m_projectionUniform = glGetUniformLocation(m_programID, "projectionMatrix");
	m_positionAttribute = glGetAttribLocation(m_programID, "v_position");
	m_uvAttribute = glGetAttribLocation(m_programID, "v_uv");
	m_colorAttribute = glGetAttribLocation(m_programID, "v_color");

	glGenBuffers(1, &m_vboID);
	glGenBuffers(1, &m_elementsID);
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableVertexAttribArray(m_positionAttribute);
	glEnableVertexAttribArray(m_uvAttribute);
	glEnableVertexAttribArray(m_colorAttribute);
	glVertexAttribPointer(m_positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, pos)));
	glVertexAttribPointer(m_uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, uv)));
	glVertexAttribPointer(m_colorAttribute, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, col)));

	//Build texture atlas based on font
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	//Upload texture to graphics system
	GLint last_texture, last_array_buffer, last_vertex_array;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	glGenTextures(1, &m_atlasTextureID);
	glBindTexture(GL_TEXTURE_2D, m_atlasTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	//Store our identifier
	io.Fonts->TexID = reinterpret_cast<void*>(static_cast<intptr_t>(m_atlasTextureID));
	//Restore state
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindVertexArray(last_vertex_array);

	//Error checking
	GLenum errCode = glGetError();
	if (errCode != GL_NO_ERROR)
	{
		fprintf(stderr, "OpenGL error = %d\n", errCode);
		assert(false);
	}

	//Setup style
	SetupStyle();
}

void Elite::EImmediateUI::Render()
{
	ImGui::Render();
}

void Elite::EImmediateUI::EventProcessing()
{
	//Bypasses the input manager for the Immediate UI - BAD :D 
	ImGuiIO& io = ImGui::GetIO();
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
		{
			if (e.wheel.y > 0)
				m_sMouseWheel = 1;
			if (e.wheel.y < 0)
				m_sMouseWheel = -1;
			return;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			if (e.button.button == SDL_BUTTON_LEFT) m_sMousePressed[0] = true;
			if (e.button.button == SDL_BUTTON_RIGHT) m_sMousePressed[1] = true;
			if (e.button.button == SDL_BUTTON_MIDDLE) m_sMousePressed[2] = true;
			return;
		}
		case SDL_TEXTINPUT:
		{
			io.AddInputCharactersUTF8(e.text.text);
			return;
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			int key = e.key.keysym.sym & ~SDLK_SCANCODE_MASK;
			io.KeysDown[key] = (e.type == SDL_KEYDOWN);
			io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
			io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
			io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
			io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			return;
		}
		}
	}
}

void Elite::EImmediateUI::StaticRender(ImDrawData* drawData)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fbWidth = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fbHeight = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fbWidth == 0 || fbHeight == 0)
		return;
	drawData->ScaleClipRects(io.DisplayFramebufferScale);

	// Backup GL state
	GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
	GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
	GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
	GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
	GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	// Setup orthographic projection matrix
	glViewport(0, 0, (GLsizei)fbWidth, (GLsizei)fbHeight);
	const float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
		{ 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
		{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
		{ -1.0f,                  1.0f,                   0.0f, 1.0f },
	};
	glUseProgram(m_programID);
	glUniform1i(m_textureUniform, 0);
	glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, &ortho_projection[0][0]);
	glBindVertexArray(m_vaoID);

	for (int n = 0; n < drawData->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = drawData->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementsID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fbHeight - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	// Restore modified GL state
	glUseProgram(last_program);
	glActiveTexture(last_active_texture);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindVertexArray(last_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	glBlendFunc(last_blend_src, last_blend_dst);
	if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
	if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
	if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
	if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void Elite::EImmediateUI::NewFrame(EliteRawWindow pWindow, float deltaTime)
{
	ImGuiIO& io = ImGui::GetIO();

	//Setup display size (every frame because of possible window resizing, if enabled)
	int width, height;
	int displayWidth, displayHeight;
	SDL_GetWindowSize(pWindow, &width, &height);
	SDL_GL_GetDrawableSize(pWindow, &displayWidth, &displayHeight);
	io.DisplaySize = ImVec2((float)width, (float)height);
	io.DisplayFramebufferScale = ImVec2(width > 0 ? ((float)displayWidth / width) : 0, height > 0 ? ((float)displayHeight / height) : 0);

	//Setup time step
	io.DeltaTime = deltaTime;

	//Setup inputs
	//(we already got mouse wheel, keyboard keys & characters from SDL_PollEvent())
	int mx, my;
	Uint32 mouseMask = SDL_GetMouseState(&mx, &my);
	if (SDL_GetWindowFlags(pWindow) & SDL_WINDOW_MOUSE_FOCUS)
		io.MousePos = ImVec2((float)mx, (float)my);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
	else
		io.MousePos = ImVec2(-1, -1);

	//If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
	io.MouseDown[0] = m_sMousePressed[0] || (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
	io.MouseDown[1] = m_sMousePressed[1] || (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
	io.MouseDown[2] = m_sMousePressed[2] || (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
	m_sMousePressed[0] = m_sMousePressed[1] = m_sMousePressed[2] = false;

	io.MouseWheel = m_sMouseWheel;
	m_sMouseWheel = 0.0f;

	//Hide OS mouse cursor if ImGui is drawing it
	SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

	//Start the frame
	ImGui::NewFrame();
}

bool Elite::EImmediateUI::FocussedOnUI()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse || io.WantCaptureKeyboard;
}

void Elite::EImmediateUI::SetClipboardText(const char* text)
{
	SDL_SetClipboardText(text);
}

const char* Elite::EImmediateUI::GetClipboardText()
{
	return SDL_GetClipboardText();
}

void Elite::EImmediateUI::SetupStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(5, 5);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 4);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(2, 4);
	style.ItemInnerSpacing = ImVec2(0, 0);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 0.0f;
	style.Alpha = 0.6f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}
#endif