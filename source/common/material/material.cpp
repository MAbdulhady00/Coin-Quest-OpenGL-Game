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
        if (sampler)
            sampler->bind(0);
        if (!depthTexture)
            return;
        glActiveTexture(GL_TEXTURE1);
        shader->set("depth_sampler", 1);
        depthTexture->bind();
        // If there is a sampler, bind it to texture unit 0
        if (sampler)
            sampler->bind(1);
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

    // This function read the material data from a json object
    void LitMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
        albedo_map = AssetLoader<Texture2D>::get(data.value("albedo", "albedo"));
        specular_map = AssetLoader<Texture2D>::get(data.value("specular", "black"));
        emissive_map = AssetLoader<Texture2D>::get(data.value("emissive", "black"));
        roughness_map = AssetLoader<Texture2D>::get(data.value("roughness", "black"));
        ambient_occlusion_map = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", "black"));
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void LitMaterial::setup() const
    {
        TintedMaterial::setup();
        // this->shader->set("alphaThreshold", this->alphaThreshold);
        // this->sampler->bind(0);
        if (albedo_map != nullptr)
        {
            // select an active texture unit -> 1
            glActiveTexture(GL_TEXTURE0);
            // bind the texture to unit 1
            this->albedo_map->bind();
            // bind the sampler to unit 1
            this->sampler->bind(0);
            shader->set("material.albedo", 0);
        }
        if (specular_map != nullptr)
        {
            // select an active texture unit -> 1
            glActiveTexture(GL_TEXTURE1);
            // bind the texture to unit 1
            this->specular_map->bind();
            // bind the sampler to unit 1
            this->sampler->bind(1);
            shader->set("material.specular", 1);
        }
        if (emissive_map != nullptr)
        {
            // select an active texture unit -> 1
            glActiveTexture(GL_TEXTURE2);
            // bind the texture to unit 1
            this->emissive_map->bind();
            // bind the sampler to unit 1
            this->sampler->bind(2);
            shader->set("material.emissive", 2);
        }
        if (ambient_occlusion_map != nullptr)
        {
            // select an active texture unit -> 1
            glActiveTexture(GL_TEXTURE3);
            // bind the texture to unit 1
            this->ambient_occlusion_map->bind();
            // bind the sampler to unit 1
            this->sampler->bind(3);
            shader->set("material.ambient_occlusion", 3);
        }
        if (roughness_map != nullptr)
        {
            // select an active texture unit -> 1
            glActiveTexture(GL_TEXTURE4);
            // bind the texture to unit 1
            this->roughness_map->bind();
            // bind the sampler to unit 1
            this->sampler->bind(4);
            shader->set("material.roughness", 4);
        }

        glActiveTexture(GL_TEXTURE0);
    }
}