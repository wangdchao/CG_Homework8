#include "allheader.h" 
#include "shader.h"
#include "Curve.h"
//#include "Bresenham.h"

//�쳣�ص�����
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


//������ɫ��
const char *vertexShaderSource1 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x*2/500, aPos.y*2/500, aPos.z*2/500, 1.0);\n"
"}";

const char *vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"   gl_Position = model*vec4(aPos.x/500, aPos.y/500, aPos.z/500, 1.0);\n"
"}";

//Ƭ����ɫ��
const char *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f,0.0f,0.0f,1.0f);\n"
"}\n";

float triangle1[] = {
	-8.0f,8.0f,0.0f,
	8.0f,8.0f,0.0f,
	-8.0f,-8.0f,0.0f,
	-8.0f,-8.0f,0.0f,
	8.0f,8.0f,0.0f,
	8.0f,-8.0f,0.0f
};

float triangle2[] = {
	-4.0f,4.0f,0.0f,
	4.0f,4.0f,0.0f,
	-4.0f,-4.0f,0.0f,
	-4.0f,-4.0f,0.0f,
	4.0f,4.0f,0.0f,
	4.0f,-4.0f,0.0f
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
GLdouble posx = 0;
GLdouble posy = 0;
Bezier curve = Bezier();
int main(int, char**)
{
	//-------------------------------------------------------------
	// Setup window�������쳣�ص�����
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	//-------------------------------------------------------------
	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
	
	//-----------------------------------------------------------------------------
	// ����һ�����ڶ���

	GLFWwindow* window = glfwCreateWindow(MAXWIDTH, MAXHEIGHT, "OpenGL3", NULL, NULL);
	if (window == NULL)
		return 1;

	//Ϊ���ڶ��󴴽�һ��������
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	//�ص�
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//---------------------------------------------------------------
	// ��ʼ��opengl����������
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	//-----------------------------------------------------------------
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//-------------------------------------------------------------------
	//������ɫ������
	shader shaderProgram=shader();
	if (!shaderProgram.createShader(vertexShaderSource1, fragmentShaderSource1)) {
		std::cout << "shader program create failly!" << std::endl;
		exit(1);
	}
	shader shaderProgram1 = shader();
	if (!shaderProgram1.createShader(vertexShaderSource2, fragmentShaderSource1)) {
		std::cout << "shader program create failly!" << std::endl;
		exit(1);
	}
	//--------------------------------------------------------------------
	//�˵�ѡ���ֵ��animation=true����ʾ�������ɶ���
	bool animation = false;
	//RGB��ɫ��������Ⱦ������ʱ����ɫ
	float ccolor[3];
	

	//������Ӧ�Ķ���������󡢶��㻺�����-�������Ρ�ֱ�ߡ���
	unsigned int VAO[5], VBO[5];
	

	//�ֱ�������������������ֱ��ȷ��������
	//drawLineWithBresenham(0, 200, -100, -100, line_point);
	//drawLineWithBresenham(0, 200, 100, -100, line_point);
	//drawLineWithBresenham(-100, -100, 100, -100, line_point);

	//�����ε���������
	int p[3][2] = {
				0,200,-100,-100,100,-100
	};
	//drawTriangleWithFilling(p, triangle_point);
	
	//�ñ��������ж�д�������ε�VBO�����ݵ���
	glGenBuffers(5, VBO);
	glGenVertexArrays(5, VAO);
	
	//����ֱ�ߣ���VAO��VBO������ֱ�ߵĶ�������д�뻺����
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//������д�뻺����
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), &triangle1, GL_STATIC_DRAW);
	//����opengl�������е����ݵĶ�ȡ��ʽ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//����ֱ�ߣ���VAO��VBO������ֱ�ߵĶ�������д�뻺����
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	//������д�뻺����
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), &triangle2, GL_STATIC_DRAW);
	//����opengl�������е����ݵĶ�ȡ��ʽ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	float t = 0.0;
	// Main loop
	while (!glfwWindowShouldClose(window))//�ж������Ƿ�رգ�����������ѭ����Ⱦ
	{
		//��ⴰ���¼��ķ�����������������������
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//�����Ǵ���һ��ͼ�ν�������
		ImGui::Begin("Hello, world!", NULL, ImGuiWindowFlags_MenuBar);              // Create a window called "Hello, world!" and append into it.
		ImGui::Checkbox("Animation", &animation);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		//�����ӿڣ������봰�ڴ�С����һ��
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, MAXWIDTH,MAXHEIGHT );
		//�������Դ������ñ���ɫ
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//ÿ��ѭ��ˢ��ͼ�ν����֡
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		for (unsigned int i = 0; i < curve.size; i++) {
			double dx = (curve.P[i]).x *2/ MAXWIDTH;
			double dy = (curve.P[i]).y *2/ MAXHEIGHT;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(dx, dy, 0.0));
			shaderProgram1.use();
			shaderProgram1.setVertexUniformModel(model);
			glBindVertexArray(VAO[1]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		float dt = 1 / (float)DOTNUM;
		t += dt;
		if (t >= 1.0) t = 0.0;
		if (curve.size > 1) {
			//������֮�������
			for (int i = 0; i < curve.size - 1; i++) {
				vector<GLdouble> line;
				line.push_back((curve.P[i]).x); line.push_back((curve.P[i]).y); line.push_back(0.0);
				line.push_back((curve.P[i+1]).x); line.push_back((curve.P[i+1]).y); line.push_back(0.0);
				glBindVertexArray(VAO[0]);
				glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble)*line.size(), &line[0], GL_STATIC_DRAW);
				//����opengl�������е����ݵĶ�ȡ��ʽ
				glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (void*)0);
				glEnableVertexAttribArray(0);
				glBindVertexArray(VAO[0]);
				shaderProgram.use();
				glDrawArrays(GL_LINES, 0, (GLint)((float)line.size() / 3));
			}
			//����Bezier����
			curve.getBezierCurve();
			std::vector<GLdouble> cur = curve.getCurve();
			glBindVertexArray(VAO[2]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble)*cur.size(), &cur[0], GL_STATIC_DRAW);
			//����opengl�������е����ݵĶ�ȡ��ʽ
			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (void*)0);
			glEnableVertexAttribArray(0);
			glBindVertexArray(VAO[2]);
			shaderProgram.use();
			glDrawArrays(GL_POINTS, 0, (GLint)((float)cur.size() / 3));

			
			if(animation) {
				curve.CalculateAnimationData(t);
				//vector<GLdouble> LL = curve.getAN();
				for (int i = 0; i < curve.Animation.size(); i++) {
					double dx = (curve.Animation[i]).x * 2 / MAXWIDTH;
					double dy = (curve.Animation[i]).y * 2 / MAXHEIGHT;
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(dx, dy, 0.0));
					shaderProgram1.use();
					shaderProgram1.setVertexUniformModel(model);
					glBindVertexArray(VAO[4]);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				
				//printf("@@@\n");
				if (curve.size > 2) {
					int allsize = curve.size - 1;
					int step = 0;
					bool st = true;
					if (allsize < 1) continue;
					else {
						while (allsize >= 1) {
							if (st) st = false;
							else step++;
							for (int i = 0; i < allsize - 1; i++) {
								vector<GLdouble> LL;
								GLdouble x1 = (curve.Animation[step]).x , y1 = (curve.Animation[step]).y;
								GLdouble x2 = (curve.Animation[step + 1]).x, y2 =(curve.Animation[step + 1]).y;
								LL.push_back(x1); LL.push_back(y1); LL.push_back(0.0);
								LL.push_back(x2); LL.push_back(y2); LL.push_back(0.0);
								glBindVertexArray(VAO[3]);
								glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
								glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble)*LL.size(), &LL[0], GL_STATIC_DRAW);
								//����opengl�������е����ݵĶ�ȡ��ʽ
								glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (void*)0);
								glEnableVertexAttribArray(0);
								glBindVertexArray(VAO[3]);
								shaderProgram.use();
								glDrawArrays(GL_LINES, 0, (GLint)((float)LL.size() / 3));
								step++;
							}
							allsize--;
						}
					}
				}
			}
		}
		Sleep(100);
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	//���ImGUI��opengl��glfw֮��Ĺ�����Դ
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	//�ͷ�imGUI�������Դ
	ImGui::DestroyContext();
	//ɾ��ǰ�������ɵ�VAO��VBO
	glDeleteVertexArrays(5, VAO);
	glDeleteBuffers(5, VBO);
	//�ͷŴ��ڶ���
	glfwDestroyWindow(window);
	//�ͷ����еĴ�����Դ
	glfwTerminate();

	return 0;
}

//����ƶ��ص�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//printf("%lf %lf\n", xpos, ypos);
	posx = xpos;
	posy = ypos;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT: {
		printf("add point\n");
		Point p = Point(posx-MAXWIDTH/2, -(posy-MAXHEIGHT/2), true);
		if (!curve.hasExistedPoint(p)) {
			p.last = true;
			curve.addPoint(p);
		}
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT: {
		Point p = Point(posx-MAXWIDTH/2, MAXHEIGHT/2-posy, false);
		bool is = curve.isLast(p);
		if (is == true) printf("true\n");
		else printf("false\n");
		if (is) {
			curve.deleteLastPoint();
		}
		break;
	}
	default:
		return;
	}
	return;
}
