#include "infobox.hh"

#include <regex>

#include <imgui.h>
#include <SDL3/SDL.h>

static void render_tag(std::string const& type, std::string const& text)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 12.f));

    if (type == "text") {
        std::istringstream ss(text);
        std::string line;
        while (std::getline(ss, line))
            ImGui::TextWrapped("%s", line.c_str());

    } else if (type == "image" || type == "image_sl") {
        if (type == "image_sl")
            ImGui::SameLine();

        ps::res::Tile tile = ps::res::tile(text);

        ImVec2 uv0 = ImVec2(tile.rect.x / tile.texture->w, tile.rect.y / tile.texture->h);
        ImVec2 uv1 = ImVec2((tile.rect.x + tile.rect.w) / tile.texture->w, (tile.rect.y + tile.rect.h) / tile.texture->h);

        ImGui::Image((ImTextureID) tile.texture, { tile.rect.w * 2, tile.rect.h * 2 }, uv0, uv1);

    } else {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "[Invalid tag `%s`]", type.c_str());
    }

    ImGui::PopStyleVar();
}


static void render_contents(std::string const& contents)
{
    static const std::regex variable_pattern(R"(\$\{([^:]+):([^}]+)\})");  // Regex to match ${xxx:yyy} pattern

    std::sregex_iterator begin(contents.begin(), contents.end(), variable_pattern);
    std::sregex_iterator end;

    size_t lastPos = 0;

    // Iterate through all matches
    for (std::sregex_iterator i = begin; i != end; ++i) {
        const std::smatch& match = *i;

        // Add any plain text before the tag
        if (match.position() > (long) lastPos) {
            std::string text = contents.substr(lastPos, match.position() - lastPos);
            render_tag("text", text);
        }

        // Add the tag
        std::string name = match[1].str();
        std::string value = match[2].str();
        // Trim whitespace
        name = std::regex_replace(name, std::regex("^\\s+|\\s+$"), "");
        value = std::regex_replace(value, std::regex("^\\s+|\\s+$"), "");
        render_tag(name, value);

        lastPos = match.position() + match.length();
    }

    // Add any remaining plain text after the last variable
    if (lastPos < contents.length()) {
        std::string text = contents.substr(lastPos);
        render_tag("text", text);
    }
}

void render_infobox(luaobj::Render const& render, std::vector<luaobj::Event>& events)
{
    constexpr float INFOBOX_WIDTH = 350.f;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 window_pos = { viewport->WorkSize.x - INFOBOX_WIDTH, viewport->WorkPos.y };
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize({ INFOBOX_WIDTH, viewport->WorkSize.y });

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 16, 16 });

    ImGui::SetNextWindowBgAlpha(0.35f);

    if (ImGui::Begin("InfoBox", nullptr, window_flags)) {
        if (render.infobox)
            render_contents(*render.infobox);
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}