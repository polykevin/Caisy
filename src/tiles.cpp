#include "tiles.h"

tiles::tiles(VAO_Class VAO) 
: shaderProgram("Resources/shaders/background.vert", "Resources/shaders/background.frag"), VBO(NULL, NULL)
{
    float tilesVertices[] = {
        // Position         // Colors,
         1.0f,  1.0f, 0.0f,  firstShadingR, firstShadingG, firstShadingB, 1.0f,
         1.0f, -1.0f, 0.0f,  firstShadingR, firstShadingG, firstShadingB, 0.3f,
        -1.0f, -1.0f, 0.0f,  secondShadingR, secondShadingG, secondShadingB, 1.0f,
        -1.0f,  1.0f, 0.0f,  secondShadingR, secondShadingG, secondShadingB, 0.3f

    };

    unsigned int tilesIndices[] = {
        0, 1, 3,
        1, 2, 3
    };


    // Initializing
    VAO.Bind();
    //Shader shaderProgram("background.vert", "background.frag");

    VBO_Class VBO(tilesVertices, sizeof(tilesVertices));
    EBO_Class EBO(tilesIndices, sizeof(tilesIndices));


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    VAO.Unbind();
}

void tiles::renderTiles()
{
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void tiles::clean()
{
    shaderProgram.Delete();
    VBO.Delete();
}