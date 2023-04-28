#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    /**
     * @brief Setup the material for rendering.
     */
    void Material::setup() const
    {
        // Setup the pipeline state
        pipelineState.setup();
        // Set the shader to be used
        shader->use();
    }

    /**
     * @brief Read the material data from a json object.
     *
     * @param data json object to read from.
     */
    void Material::deserialize(const nlohmann::json &data)
    {
        // This function read the material data from a json object
        if (!data.is_object())
            return;

        // check if the data contains the key "pipelineState"
        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        // set the shader
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        // set the transparent flag
        transparent = data.value("transparent", false);
    }

    /**
     * @brief Setup the tinted material for rendering.
     */
    void TintedMaterial::setup() const
    {

        // Call the setup of the parent
        Material::setup();
        // Set the "tint" uniform
        shader->set("tint", tint);
    }

    /**
     * @brief Read the material data from a json object.
     *
     * @param data json object to read from.
     */
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        // This function read the material data from a json object
        Material::deserialize(data);
        if (!data.is_object())
            return;
        // set the tint
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    /**
     * @brief Setup the texture material for rendering.
     */
    void TexturedMaterial::setup() const
    {
        // This function should call the setup of its parent and
        // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
        // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"

        // Call the setup of the parent
        TintedMaterial::setup();
        // Set the "alphaThreshold" uniform
        shader->set("alphaThreshold", alphaThreshold);
        // Set active texture unit to 0
        glActiveTexture(GL_TEXTURE0);
        // Bind texture to texture unit 0
        shader->set("tex", 0);
        texture->bind();
        // If there is a sampler, bind it to texture unit 0
        if (sampler)
            sampler->bind(0);
    }

    /**
     * @brief Read the material data from a json object.
     *
     * @param data json object to read from.
     */
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        // This function read the material data from a json object
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}