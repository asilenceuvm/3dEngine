#include "scene.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "python38/Python.h"

#include "resourceManager.h"
#include "inputManager.h"

glm::vec3 Scene::lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
std::map<std::string, std::shared_ptr<Model>> Scene::models;
std::map<std::string, GameObject> Scene::gameObjects;

void Scene::initRes() {
	ResourceManager::getShader("lighted").setInt("material.diffuse", 0);
	ResourceManager::getShader("lighted").setInt("material.specular", 1);
	ResourceManager::getShader("lighted").setFloat("material.shine", 256.0f);
	
	ResourceManager::getShader("lighted").setVec3("dirLight.direction", 0.0f, -0.0f, -1.0f);
	ResourceManager::getShader("lighted").setVec3("dirLight.ambient", 0.7f, 0.7f, 0.7f);
	ResourceManager::getShader("lighted").setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	ResourceManager::getShader("lighted").setVec3("dirLight.specular", 0.3f, 0.3f, 0.3f);

	ResourceManager::getShader("lighted").setVec3("pointLight.position", 0.0f, 1.0f, -200.0f);
	ResourceManager::getShader("lighted").setFloat("pointLight.constant", 1.0f);
	ResourceManager::getShader("lighted").setFloat("pointLight.linear", 0.09f);
	ResourceManager::getShader("lighted").setFloat("pointLight.quadratic", 0.032f);
	ResourceManager::getShader("lighted").setVec3("pointLight.ambient", 0.2f, 0.2f, 0.2f);
	ResourceManager::getShader("lighted").setVec3("pointLight.diffuse", 0.5f, 0.5f, 0.5f);
	ResourceManager::getShader("lighted").setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);

	ResourceManager::getShader("lighted").setVec3("objectColor", 0.8f, 0.8f, 0.8f);
	ResourceManager::getShader("lighted").setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	ResourceManager::getShader("lighted").setVec3("lightPos", 0.0f, 1.0f, -3.0f);

	auto model = std::make_shared<Model>("res/models/pier/woodenpier.obj");
	models.insert({"pier", model });

	GameObject go(models.find("pier")->second);
	gameObjects.insert({ "pier", go });
	//model2 = std::make_unique<Model>("res/models/cage/Cage.obj");

	std::vector<std::string> faces {
        "res/textures/skybox/clouds1_east.bmp",
        "res/textures/skybox/clouds1_west.bmp",
        "res/textures/skybox/clouds1_up.bmp",
        "res/textures/skybox/clouds1_down.bmp",
        "res/textures/skybox/clouds1_north.bmp",
        "res/textures/skybox/clouds1_south.bmp"
    };
	skybox = std::make_unique<SkyBox>(faces);

	shadowRenderer = std::make_unique<ShadowRenderer>();
}

static PyObject* engine_move(PyObject* self, PyObject* args) {
	float x, y, z;
	if(PyArg_ParseTuple(args, "fff", &x, &y, &z)) {
		Scene::lightPos = glm::vec3(x, y, z);
	}

	return PyLong_FromLong(0);
}

static PyObject* engine_add_game_object(PyObject* self, PyObject* args) {
	char* name;
	char* modelname;
	if(PyArg_ParseTuple(args, "ss", &name, &modelname)) {
		Scene::gameObjects.insert({name, Scene::models.find(modelname)->second});
	}

	return PyLong_FromLong(0);
}

static PyObject* engine_move_game_object(PyObject* self, PyObject* args) {
	char* name;
	float x, y, z;
	if(PyArg_ParseTuple(args, "sfff", &name, &x, &y, &z)) {
		Scene::gameObjects.find(name)->second.setPos(glm::vec3(x, y, z));
	}

	return PyLong_FromLong(0);
}

static PyObject* engine_rotate_game_object(PyObject* self, PyObject* args) {
	char* name;
	float x, y, z;
	if(PyArg_ParseTuple(args, "sfff", &name, &x, &y, &z)) {
		Scene::gameObjects.find(name)->second.setRotate(glm::vec3(x, y, z));
	}

	return PyLong_FromLong(0);
}

static PyObject* engine_scale_game_object(PyObject* self, PyObject* args) {
	char* name;
	float x, y, z;
	if(PyArg_ParseTuple(args, "sfff", &name, &x, &y, &z)) {
		Scene::gameObjects.find(name)->second.setScale(glm::vec3(x, y, z));
	}

	return PyLong_FromLong(0);
}

static struct PyMethodDef methods[] = {
	{ "move", engine_move, METH_VARARGS, "move light source"},
	{ "add_game_object", engine_add_game_object, METH_VARARGS, "add game object to scene"},
	{ "move_game_object", engine_move_game_object, METH_VARARGS, "move gameobject"},
	{ "rotate_game_object", engine_rotate_game_object, METH_VARARGS, "rotate gameobject"},
	{ "scale_game_object", engine_scale_game_object, METH_VARARGS, "scale gameobject"},
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef modDef = {
	PyModuleDef_HEAD_INIT, "engine", NULL, -1, methods, 
	NULL, NULL, NULL, NULL
};

static PyObject* PyInit_engine(void) {
	return PyModule_Create(&modDef);
}

Scene::Scene(int width, int height) {
	initRes();

	this->width = width;
	this->height = height;

	//starting values for camera
	InputManager::xoffset = 0;
	InputManager::yoffset = 0;
	camera.rotateCamera(-90, 0, 1);

	PyImport_AppendInittab("engine", &PyInit_engine);
}

void Scene::update() {
	//temp
	if (InputManager::keys[GLFW_KEY_W]) {
		camera.moveCamForward(0.05f);
	}
	if (InputManager::keys[GLFW_KEY_S]) {
		camera.moveCamBack(0.05f);
	}
	if (InputManager::keys[GLFW_KEY_A]) {
		camera.moveCamLeft(0.05f);
	}
	if (InputManager::keys[GLFW_KEY_D]) {
		camera.moveCamRight(0.05f);
	}
	camera.rotateCamera(InputManager::xoffset, InputManager::yoffset, 0.1f);
	InputManager::xoffset = 0;
	InputManager::yoffset = 0;
	glm::vec3 cameraPos = camera.getCameraPos();
	glm::vec3 cameraFront = camera.getCameraFront();
	glm::vec3 cameraUp = camera.getCameraUp();
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//temp
	if (InputManager::keys[GLFW_KEY_F]) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	}

	if (InputManager::keys[GLFW_KEY_V]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}


}

void Scene::render() {
	//first pass
	//lightPos = glm::vec3(sin(glfwGetTime()), 3, 2);

	Shader s = ResourceManager::getShader("shadow");
	shadowRenderer->render(s, lightPos);
	renderScene(s);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//second pass
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	ResourceManager::getShader("lighted").use();
	ResourceManager::getShader("lighted").setMat4("projection", projection);
	ResourceManager::getShader("lighted").setMat4("view", view);
	ResourceManager::getShader("lighted").setVec3("viewPos", camera.getCameraPos());
	ResourceManager::getShader("lighted").setVec3("lightPos", lightPos);
	ResourceManager::getShader("lighted").setMat4("lightSpaceMatrix", shadowRenderer->getLightSpaceMatrix());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowRenderer->getDepthMap());

	s = ResourceManager::getShader("lighted");
	renderScene(s);

	//draw skybox
	s = ResourceManager::getShader("skybox");
	skybox->render(s, view);
}

void Scene::renderScene(Shader& shader) {
	//model->render(shader, glm::vec3(0,0,0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 0, 0));
	//models[0]->render(shader, glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 0, 0));
	//model2->render(shader, glm::vec3(0.5, 1.2, 0), glm::vec3(0.01), glm::vec3(-1, 0, 0));
	for (auto& x : gameObjects) {
		x.second.render(shader);

	}
}

