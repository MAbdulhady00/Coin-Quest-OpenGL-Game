#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
namespace our
{

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json &config)
    {
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if (config.contains("sky"))
        {
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram *skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();

            // pipeline state for sky
            PipelineState skyPipelineState;
            skyPipelineState.depthTesting.enabled = true;
            // since the sphere is drawn far away (z = 1).
            skyPipelineState.depthTesting.function = GL_LEQUAL;
            skyPipelineState.faceCulling.enabled = true;
            // cull the front face since we are inside the sphere
            skyPipelineState.faceCulling.culledFace = GL_FRONT;

            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky
            Sampler *skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if (config.contains("postprocess"))
        {
            if (const auto &shaders = config["postprocess"]; shaders.is_array())
            {
                // use postprocessframebuffer of forwardrenderer as our framebuffer
                glGenFramebuffers(1, &postprocessFrameBuffer);

                //  Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
                //  The depth format can be (Depth component with 24 bits).
                // the enums of GL_RGBA8 and GL_DEPTH_COMPONENT24 didn't work, so I used GL_RGBA and GL_DEPTH_COMPONENT instead
                this->depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT, this->windowSize);
                this->colorTarget = texture_utils::empty(GL_RGBA, this->windowSize);

                // bind the framebuffer before attach our textures
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
                // attach the color and depth texture to the framebuffer
                glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTarget->getOpenGLName(), 0);
                glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTarget->getOpenGLName(), 0);
                // Unbind the framebuffer just to be safe
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

                // Create a vertex array to use for drawing the texture
                glGenVertexArrays(1, &postProcessVertexArray);
                int i = 0;
                for (auto &shader : shaders)
                {

                    // Create a sampler to use for sampling the scene texture in the post processing shader
                    Sampler *postprocessSampler = new Sampler();
                    postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                    // Create the post processing shader
                    ShaderProgram *postprocessShader = new ShaderProgram();
                    postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);

                    postprocessShader->attach(shader, GL_FRAGMENT_SHADER);

                    postprocessShader->link();

                    // Create a post processing material
                    this->postprocessMaterials.push_back(new TexturedMaterial());
                    this->postprocessMaterials[i]->shader = postprocessShader;
                    this->postprocessMaterials[i]->texture = this->colorTarget;
                    this->postprocessMaterials[i]->depthTexture = this->depthTarget;
                    this->postprocessMaterials[i]->sampler = postprocessSampler;

                    // The default options are fine but we don't need to interact with the depth buffer
                    // so it is more performant to disable the depth mask
                    this->postprocessMaterials[i]->pipelineState.depthMask = false;
                    i++;
                }
            }
        }
    }

    void ForwardRenderer::destroy()
    {
        // Delete all objects related to the sky
        if (skyMaterial)
        {
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if (postprocessMaterials.size() > 0)
        {
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
        }
        for (auto postprocessMaterial : postprocessMaterials)
        {
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World *world)
    {
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent *camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();

        for (auto entity : world->getEntities())
        {
            // If we hadn't found a camera yet, we look for a camera in this entity
            if (!camera)
                camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
            {
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if (command.material->transparent)
                {
                    transparentCommands.push_back(command);
                }
                else
                {
                    // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
            auto light = entity->getComponent<LightComponent>();
            if (light && light->enabled)
            {
                if (light->typeLight == LightType::SKY)
                {
                    auto litShader = AssetLoader<ShaderProgram>::get("light");
                    litShader->use();
                    litShader->set("sky.top", glm::vec3(0.0f, 0.1f, 0.5f));
                    litShader->set("sky.horizon", glm::vec3(0.3f, 0.3f, 0.3f));
                    litShader->set("sky.bottom", glm::vec3(0.1f, 0.1f, 0.1f));
                }
                else
                    lights.push_back(light);
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if (camera == nullptr)
            return;

        // get the camera forward direction in world space looking at the negative z axis
        glm::vec3 cameraForward = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0.0, 0.0, -1.0f, 1.0f);
        // sort the transparent commands based on the distance from the camera
        // this is done to prevent the transparent objects from being drawn in the wrong order
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand &first, const RenderCommand &second)
                  { 
            // get the distance from the camera to the center of the first and second commands
            return glm::dot(first.center, cameraForward) < glm::dot(second.center, cameraForward); });

        // get the camera view projection matrix
        glm::mat4 VP = camera->getProjectionMatrix(windowSize) * camera->getViewMatrix();

        // set the viewport to the size of the window
        glViewport(0, 0, windowSize.x, windowSize.y);

        // set the clear color to black and the clear depth to 1
        glClearColor(0, 0, 0, 1);
        glClearDepth(1);

        // enable writing to the color and depth buffers
        glColorMask(true, true, true, true);
        glDepthMask(true);

        // If there is a postprocess material, bind the framebuffer
        if (postprocessMaterials.size() > 0)
        {
            // bind the framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
        }

        // clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // define the max number of lights
        const int MAX_LIGHT_COUNT = 8;

        // draw all the opaque commands
        // no need to sort them because of the z-buffer
        for (auto &command : opaqueCommands)
        {
            command.material->setup();
            // set the camera position
            command.material->shader->set("camera_position", cameraForward);
            // set the M matrix for the shader
            command.material->shader->set("M", command.localToWorld);
            // set the M_IT matrix for the shader
            command.material->shader->set("M_IT", glm::transpose(glm::inverse(command.localToWorld)));
            // set the VP matrix for the shader
            command.material->shader->set("VP", VP);
            // set the MVP matrix for the shader
            command.material->shader->set("transform", VP * command.localToWorld);
            // set the light count
            command.material->shader->set("light_count", static_cast<int>(lights.size()));
            // set the light properties
            int light_index = 0;
            // add the light effects
            for (auto light : lights)
            {
                if (light->typeLight == LightType::SKY)
                    continue;
                if (!light->enabled)
                    continue;
                light->position = light->getOwner()->getWorldTranslation();
                light->direction = light->getOwner()->getLocalToWorldMatrix() * glm::vec4(light->direction, 0.0);
                // std::cout<< "Light direction: " << light->direction.x << " " << light->direction.y << " " << light->direction.z << std::endl;
                // std::cout<<"Light position: "<<light->position.x<<" "<<light->position.y<<" "<<light->position.z<<std::endl;

                std::string prefix = "lights[" + std::to_string(light_index) + "].";

                command.material->shader->set(prefix + "type", static_cast<int>(light->typeLight));
                command.material->shader->set(prefix + "color", light->color);
                switch (light->typeLight)
                {
                case LightType::DIRECTIONAL:
                    command.material->shader->set(prefix + "direction", light->direction);
                    break;
                case LightType::POINT:
                    command.material->shader->set(prefix + "position", light->position);
                    command.material->shader->set(prefix + "attenuation", glm::vec3(light->attenuation.quadratic,
                                                                                    light->attenuation.linear, light->attenuation.constant));
                    break;
                case LightType::SPOT:
                    command.material->shader->set(prefix + "position", light->position);
                    command.material->shader->set(prefix + "direction", light->direction);
                    command.material->shader->set(prefix + "attenuation", glm::vec3(light->attenuation.quadratic,
                                                                                    light->attenuation.linear, light->attenuation.constant));
                    command.material->shader->set(prefix + "cone_angles", glm::vec2(light->spot_angle.inner, light->spot_angle.outer));
                    break;
                case LightType::SKY:
                    break;
                }
                light_index++;
                if (light_index >= MAX_LIGHT_COUNT)
                    break;
            }

            command.mesh->draw();
        }

        // If there is a sky material, draw the sky
        if (this->skyMaterial)
        {
            skyMaterial->setup();
            // get camera position in world space
            glm::vec3 cameraPosition = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            // move the sky sphere to the camera position
            glm::mat4 skyModel = glm::translate(glm::mat4(1.0f), cameraPosition);

            // make the sky sphere always behind everything else
            // this is done by setting the z component of the position to 1
            // by scaling z to 0 then translate it to 1
            glm::mat4 alwaysBehindTransform = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f);
            // set the MVP matrix for the shader
            skyMaterial->shader->set("transform", alwaysBehindTransform * VP * skyModel);
            // draw the sky sphere
            skySphere->draw();
        }

        // draw all the transparent commands
        for (auto &command : transparentCommands)
        {
            command.material->setup();
            // set the MVP matrix for the shader
            command.material->shader->set("transform", VP * command.localToWorld);
            command.mesh->draw();
        }
        // If there is a postprocess material, apply postprocessing
        for (int i = 0; i < postprocessMaterials.size() - 1; ++i)
        {
            // this was done by unbinding the postprocessFrameBuffer
            // setup postprocess material
            postprocessMaterials[i]->setup();

            postprocessMaterials[i]->shader->set("inverse_projection", glm::inverse(camera->getProjectionMatrix(windowSize)));

            // bind the postprocess vertex array to draw vertices
            glBindVertexArray(postProcessVertexArray);
            // draw the vertices
            glDrawArrays(GL_TRIANGLES, 0, 3);
            this->colorTarget = postprocessMaterials[i]->texture;
            this->depthTarget = postprocessMaterials[i]->depthTexture;
            // cascade the textures to the next postprocess material
            if (i + 1 < postprocessMaterials.size())
            {
                postprocessMaterials[i + 1]->texture = this->colorTarget;
                // postprocessMaterials[i + 1]->depthTexture = this->depthTarget;
            }
        }
        if (postprocessMaterials.size() > 0)
        {
            // this was done by unbinding the postprocessFrameBuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            int end = postprocessMaterials.size() - 1;
            postprocessMaterials[end]->texture = this->colorTarget;
            postprocessMaterials[end]->depthTexture = this->depthTarget;

            postprocessMaterials[end]->setup();

            postprocessMaterials[end]->shader->set("inverse_projection", glm::inverse(camera->getProjectionMatrix(windowSize)));

            // bind the postprocess vertex array to draw vertices
            glBindVertexArray(postProcessVertexArray);
            // draw the vertices
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }

}