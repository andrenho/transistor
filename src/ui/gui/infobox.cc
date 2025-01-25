#include "infobox.hh"

#include <regex>

#include "imgui.h"
#include "ui/ui.hh"

void Infobox::init()
{

}

void Infobox::render() const
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
        if (ui().state().selected_component) {
            auto o_infobox_text = game().infobox_text(*ui().state().selected_component);
            if (o_infobox_text)
                render_contents(*o_infobox_text);
        }
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}

void Infobox::render_contents(std::string const& contents) const
{
    // TODO - make those regex replacement in initialization
    static const std::regex leading_whitespace(R"(^\s+)");     // matches leading whitespace
    static const std::regex trailing_whitespace(R"(\s+$)");    // matches trailing whitespace
    static const std::regex multiple_spaces(R"([ \t]{2,})");      // matches 2 or more whitespace characters
    static const std::regex enter_and_space(R"(\n[ \t])");
    static const std::regex variable_pattern(R"(\$\{([^:]+):([^}]+)\})");  // Regex to match ${xxx:yyy} pattern

    std::string result = contents;
    result = std::regex_replace(result, leading_whitespace, "");
    result = std::regex_replace(result, multiple_spaces, " ");
    result = std::regex_replace(result, enter_and_space, "\n");
    result = std::regex_replace(result, trailing_whitespace, "");

    std::sregex_iterator begin(result.begin(), result.end(), variable_pattern);
    std::sregex_iterator end;

    size_t lastPos = 0;

    // Iterate through all matches
    for (std::sregex_iterator i = begin; i != end; ++i) {
        const std::smatch& match = *i;

        // Add any plain text before the tag
        if (match.position() > (long) lastPos) {
            std::string text = result.substr(lastPos, match.position() - lastPos);
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
    if (lastPos < result.length()) {
        std::string text = result.substr(lastPos);
        render_tag("text", text);
    }
}

void Infobox::render_tag(std::string const& type, std::string const& text) const
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

        Tile tile = res().tile(text);

        int tx_w, tx_h;
        SDL_QueryTexture(tile.texture, nullptr, nullptr, &tx_w, &tx_h);
        ImVec2 uv0 = ImVec2( (float) tile.x / tx_w, (float) tile.y / tx_h );
        ImVec2 uv1 = ImVec2( (float)(tile.x + tile.w) / tx_w, (float)(tile.y + tile.h) / tx_h );

        ImGui::Image((ImTextureID) tile.texture, { (float) tile.w * 2, (float) tile.h * 2 }, uv0, uv1);

    } else {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "[Invalid tag `%s`]", type.c_str());
    }

    ImGui::PopStyleVar();
}
