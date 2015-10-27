#include <transformation.hpp>

#include "glfwWindow.h"

#include "customObj.hpp"

#include "objloader.hpp"
#include "shapes.hpp"
#include "scene.hpp"
#include "renderer.hpp"
#include "projectiveCamera.hpp"
#include "orthoCamera.hpp"

#include "square2D.h"
#include "math2D.h"
using namespace threemonkeybits;

#include <string>

class RectGenerator : public GLFWWindowImpl
{
public:
    RectGenerator();
    ~RectGenerator();

    // Scene management
    bool initScene();
    virtual bool updateScene();
    virtual bool render();
    void close();

    // Quads
    bool generateQuads(bool renderEach = false, unsigned int delayMs = 1000);		///< Algorithm Step 1, generate abstract quads
    bool separateQuads();
    bool createHallways();
    bool selectRooms();			///< Algorithm Step 3, according a threshold, consider some quads rooms, rest are simple cells
    bool createRoomGraph();		///< Algorithm Step 4, create a graph using the rooms center using Delaunay Triangulation
    bool createConnections();	///< Algorithm Step 5, use minimal spanning tree to guarrantees all rooms connected but not each one with the rest
    bool addLoops();			///< Algorithm Step 6, add loops adding a % of removed elements
    bool createCorridors();		///< Algorithm Step 7, create corridors in L shape between center rooms adding remaining cells to them
    void destroyQuads();		///< Algorithm final step, DESTROY!

    // Quads helpers
	bool quadOverlap(Square2D* square1, Square2D* square2);
	std::vector<Square2D*> quadOverlapWith(Square2D* square);
	void printQuadsInfo();		///< Helper to print info in log

    // Create & destroy nodes in the scene from objects
    void createNodeQuads();		///< Create 3D Scene orthogonal
    void updateNodeQuads();		///< Update 3D Scene if quads change
    void destroyNodeQuads();	///< Destroy 3D Scene orthogonal and free memory
    void createBackgroundQuad();///< Create a Quad grouping all bottom in the z depth
    gre::CustomObj* createNodeQuad(std::string name, float x0, float y0, float width, float height, float z); ///< Create a single quad in scene

    // Scene management
    void getCameraPosition(glm::vec3& pos);
    void setCameraPosition(glm::vec3& pos);
    void translateCameraPosition(const glm::vec3& offset);
    void zoom(const float value);

    float getInitialZoom() { return m_initialZoom; };

protected:
    // Assets
    std::string m_assets_path;
    std::string m_gShader;
    std::string m_vShader;
    std::string m_fShader;
    std::string m_fColourShader;
    std::string m_fWaveShader;
    std::string m_fGradientShader;
    std::string m_uvtemplate;
    std::string m_modelFile;

    std::string m_gShaderTron;
    std::string m_vShaderTron;
    std::string m_fShaderTron;

    // Rendering elements
    gre::Renderer* m_renderer;
    gre::Obj* m_base;
    gre::CustomObj* m_background;
    gre::Scene m_scene;
    gre::OrthoCamera m_camera;

    // Scene generated configuration
    int m_initialZoom;
    bool m_quadsGenerated;
    int m_maxNumberOfRectangles;
    int m_lowWidth;
    int m_highWidth;
    int m_lowHeight;
    int m_highHeight;
    int m_roomSize;
    Point2D m_centerSquares;
    float m_radiusSquares;
    std::vector<Square2D*> m_rectangles;	///< All quads
    std::vector<Square2D*> m_rooms;			///< Quads selected as rooms
    std::vector<gre::CustomObj*> m_objs;	///< All scene quads
    bool m_nodeQuadsGenerated;

};
