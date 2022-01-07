#pragma once

static const char* DefaultVertexShaderSource =
"#version 400\n"
"// Input vertex data\n"
"uniform mat4 projectionMatrix;\n"
"layout(location = 0) in vec3 v_position;\n"
"layout(location = 1) in vec4 v_color;\n"
"layout(location = 2) in float v_size;\n"
"// Output vertex data\n"
"out vec4 f_color;\n"
"void main(void)\n"
"{\n"
"	f_color = v_color;\n"
"	gl_PointSize = v_size;\n"
"	gl_Position = projectionMatrix * vec4(v_position.xy, 0.0f, 1.0f);\n"
"	gl_Position.z = v_position.z;\n"
"}\n";

static const char* DefaultFragmentShaderSource =
"#version 400\n"
"// Input data\n"
"in vec4 f_color;\n"
"// Output data\n"
"out vec4 color;\n"
"void main(void)\n"
"{ color = f_color; }\n";

static const char* ImGuiVertexShaderSource =
"#version 400\n"
"// Input vertex data\n"
"uniform mat4 projectionMatrix;\n"
"layout(location = 0) in vec2 v_position;\n"
"layout(location = 1) in vec2 v_uv;\n"
"layout(location = 2) in vec4 v_color;\n"
"// Output vertex data\n"
"out vec2 f_uv;\n"
"out vec4 f_color;\n"
"void main(void)\n"
"{\n"
"	f_uv = v_uv;\n"
"	f_color = v_color;\n"
"	gl_Position = projectionMatrix * vec4(v_position.xy, 0.0f, 1.0f);\n"
"}\n";

static const char* ImGuiFragmentShaderSource =
"#version 400\n"
"// Input data\n"
"uniform sampler2D _texture;\n"
"in vec2 f_uv;\n"
"in vec4 f_color;\n"
"// Output data\n"
"out vec4 color;\n"
"void main(void)\n"
"{ color = f_color * texture(_texture, f_uv.st); }\n";