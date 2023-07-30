//
// Created by vini84200 on 7/29/23.
//

#ifndef TRIANGLE_MATERIAL_H
#define TRIANGLE_MATERIAL_H

#include "Texture.h"
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <vector>

struct Material {
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};
    float shine{};
    std::optional<Texture> diffTexture;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
             float shine)
        : ambient(ambient), diffuse(diffuse), specular(specular),
          shine(shine){};

    Material(glm::vec3 ambient, std::string diffuseTex, glm::vec3 specular,
             float shine)
        : ambient(ambient), diffTexture({diffuseTex}), specular(specular),
          shine(shine){};

    Material() = default;

    void setDiffuseTexture(std::string texturePath) {
        diffTexture = {texturePath};
    }

    void setDiffuseTexture(Texture texture) { diffTexture = {texture}; }

    void renderImGui() {
        ImGui::ColorEdit3("Ambient", (float *)&ambient);
        ImGui::ColorEdit3("Diffuse", (float *)&diffuse);
        ImGui::ColorEdit3("Specular", (float *)&specular);
        ImGui::DragFloat("Shine", (float *)&shine);
        if (diffTexture.has_value()) {
            Texture &texture = diffTexture.value();
            ImGui::Text("Texture:");
            ImGui::Image(texture.getTextureId(), {100, 100});
            { // Texture Sampler Combo
                const char *texture_samplers[] = {"Nearest Neighbour",
                                                  "Bilinear", "Trilinear"};
                if (ImGui::BeginCombo(
                        "Texture Sampler",
                        texture_samplers[(int)texture.getTextureSampler()])) {
                    for (int i = 0; i < IM_ARRAYSIZE(texture_samplers); i++) {
                        const bool is_selected =
                            ((int)texture.getTextureSampler() == i);
                        if (ImGui::Selectable(texture_samplers[i], is_selected))
                            texture.setTextureSampler((TextureSampler)i);

                        if (is_selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
            }
            {
                // Select Texture
                const std::string textures[] = {"checker_8x8.jpg",
                                                "mandrill_256.jpg"};
                ImGui::Text("Current Texture: %s",
                            texture.getTexturePath().c_str());
                if (ImGui::Button("Select Texture"))
                    ImGui::OpenPopup("Select Texture");

                if (ImGui::BeginPopupModal("Select Texture", NULL,
                                           ImGuiWindowFlags_AlwaysAutoResize)) {
                    for (int i = 0; i < IM_ARRAYSIZE(textures); i++) {
                        if (ImGui::Selectable(textures[i].c_str())) {
                            setDiffuseTexture("assets/" + textures[i]);
                        }
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::Text("Mipmaps: %s %d", texture.hasMipmaps() ? "Yes" : "No",
                        texture.getMipmaps());
            ImGui::Text("Texture ID: %d", texture.getTextureId());
            ImGui::Text("Dimensions: %d x %d", texture.getWidth(),
                        texture.getHeight());
        } else {
            ImGui::Text("No Texture for this material.");
        }
    }
};
#endif // TRIANGLE_MATERIAL_H
