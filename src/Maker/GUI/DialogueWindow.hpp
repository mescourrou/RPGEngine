#pragma once

#include <Window.hpp>
#include <memory>
#include <Maker.hpp>

#include <ImNodes.h>
#include <ImNodesEz.h>
#include <Dialogue.hpp>

namespace maker::gui
{

class DialogueWindow : public ImGui::Window
{
  public:
    explicit DialogueWindow(Maker* maker);

  protected:
    bool doPrepare() override;
  private:
    void updateCharacterNodes();
    struct Node
    {
        Node() = default;
        virtual ~Node() = default;
        ImVec2 pos{50, 50};
        bool selected = false;
        std::vector<ImNodes::Ez::SlotInfo> inputs;
        std::vector<ImNodes::Ez::SlotInfo> outputs;
    };
    struct CharacterNode : public Node
    {
        CharacterNode() = default;
        ~CharacterNode() override = default;
        Maker::CharacterInformations characterInformation;
    };
    struct DialogueNode : public Node
    {
        DialogueNode() = default;
        ~DialogueNode() override = default;
        std::shared_ptr<quest::DialogueLine> dialogueLine;
    };

    void displayNode(std::weak_ptr<Node> node) const;
    void loadNPCDialogue(CharacterNode& node);
    std::shared_ptr<DialogueNode> loadDialogueLineRecursive(
        std::shared_ptr<quest::DialogueLine> line);

    Maker* m_maker = nullptr;
    ImNodes::CanvasState m_canvas;
    std::list<std::shared_ptr<Node>> m_nodes;
    using ConnectionTuple = std::tuple<Node*, std::string, Node*, std::string>;
    std::list<ConnectionTuple> m_connections;
    std::multimap<std::string, quest::Dialogue> m_dialogues;
};


} // namespace maker::gui
