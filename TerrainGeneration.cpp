#include "Window.h"
#include "Shader.h"
#include "Gui.h"
#include "GroundPlane.h"
#include "DirectionalLight.h"

#include <glm/glm.hpp>

int main()
{
    int length = 200, width = 200;

    char* vert_fname = (char*)"shaders/shader.vert";
    char* geo_fname = (char*)"shaders/shader.geo";
    char* frag_fname = (char*)"shaders/shader.frag";
    Shader ls = Shader();
    Window w = Window((char*)"Terrain Explorer", 1280, 720, length, width);
    w.intialize();
    GroundPlane gp = GroundPlane(length, width);
    
    gp.createMesh();
    if (!ls.loadShaderCode(vert_fname, geo_fname, frag_fname)) {
        printf("Could not load shader\n");
        return 1;
    }
    else {
        ls.useShader();
        printf("loaded land shader successfully\n");
    }
    
    DirectionalLight dirlight = DirectionalLight(ls.getShaderId());
    dirlight.setColor(1.0f, 1.0f, 1.0f);
    dirlight.setDirection(0.0f, 0.0f, 1.0f);
    dirlight.setIntensity(0.2f, 0.5f);

    ls.setProjectionUniform(w.getProjectionMatrix());


    printf("starting main loop\n");
    while (!w.shouldClose()) {
        w.newFrame();

        ls.setViewUniform(w.getViewMatrix());
        ls.setModelUniform(w.getModelMatrix());
        ls.setHeightScalarUniform(w.gui->getHeightScalarValue());
        ls.setNoiseScalarUniform(w.gui->getNoiseScalarValue());
        ls.setNoiseXOffsetUniform(w.gui->getXOffset());
        ls.setNoiseZOffsetUniform(w.gui->getZOffset());
        ls.setSteepnessExponentUniform(w.gui->getSteepnessValue());
        ls.setResolution(w.gui->getResolution());

        dirlight.setColor(w.gui->getDirectionalLightColor());
        dirlight.setDirection(w.gui->getDirectionalLightDirection());
        dirlight.setIntensity(w.gui->getDirectionalLightAmbientIntensity(), w.gui->getDirectionalLightDiffuseIntensity());

        dirlight.useLight();
        gp.render(w.gui->getRenderType());
        
        w.drawFrame();
    }
    printf("exited main loop\n");

    w.~Window();
    printf("Destroyed main window\n");
    printf("done\n");

    return 0;
}
