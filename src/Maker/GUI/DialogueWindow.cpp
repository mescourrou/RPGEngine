#include "DialogueWindow.hpp"

#include <glog/logging.h>

namespace maker::gui
{

DialogueWindow::DialogueWindow(Maker* maker) :
    Window("Dialogue"), m_maker(maker)
{

}


bool DialogueWindow::doPrepare()
{
    ImNodes::BeginCanvas(&m_canvas);

    updateCharacterNodes();

    for (auto& node : m_nodes)
    {
        displayNode(node);
    }

    for (auto& connection : m_connections)
    {
        ImNodes::Connection(std::get<0>(connection), std::get<1>(connection).c_str(),
                            std::get<2>(connection), std::get<3>(connection).c_str());
    }

    ImNodes::EndCanvas();
    return true;
}

void DialogueWindow::updateCharacterNodes()
{
    auto characterNameList = m_maker->characterList();

    for (auto& charName : characterNameList)
    {
        auto find_predicate = [charName](std::weak_ptr<Node> node)
        {
            auto charNode = std::dynamic_pointer_cast<CharacterNode>(node.lock());
            if (charNode) return charNode->characterInformation.name == charName;
            else return false;
        };
        if (std::find_if(begin(m_nodes), end(m_nodes), find_predicate) == end(m_nodes))
        {
            m_nodes.emplace_back(std::make_shared<CharacterNode>());
            auto newNode = std::static_pointer_cast<CharacterNode>(m_nodes.back());
            m_maker->getCharacterInformations(charName, newNode->characterInformation);
            // We only keep the NPCs
            if (!newNode->characterInformation.isNPC())
            {
                m_nodes.pop_back();
                continue;
            }

            newNode->pos = {50, 50};
            newNode->selected = false;
            newNode->outputs.push_back({"Dialogue", 1});
            loadNPCDialogue(*newNode);
        }


    }
}

void DialogueWindow::displayNode(std::weak_ptr<DialogueWindow::Node> node) const
{
    std::weak_ptr<CharacterNode> nodeAsCharacter =
        std::dynamic_pointer_cast<CharacterNode>(node.lock());
    std::string nodeTitle = "";
    if (nodeAsCharacter.lock())
    {
        nodeTitle = nodeAsCharacter.lock()->characterInformation.name;
    }
    std::weak_ptr<DialogueNode> nodeAsDialogue =
        std::dynamic_pointer_cast<DialogueNode>(node.lock());
    if (nodeAsDialogue.lock())
    {
        nodeTitle = nodeAsDialogue.lock()->dialogueLine.lock()->line();
    }
    if (ImNodes::Ez::BeginNode(node.lock().get(), nodeTitle.c_str(),
                               &node.lock()->pos, &node.lock()->selected))
    {
        ImNodes::Ez::InputSlots(node.lock()->inputs.data(), node.lock()->inputs.size());
        ImNodes::Ez::OutputSlots(node.lock()->outputs.data(),
                                 node.lock()->outputs.size());
        ImNodes::Ez::EndNode();
    }
}

void DialogueWindow::loadNPCDialogue(CharacterNode& node)
{
    for (const auto& dialogue : node.characterInformation.dialogueList)
    {
        std::weak_ptr<const quest::DialogueLine> firstLine = dialogue->firstLine();
        auto newNode = loadDialogueLineRecursive(firstLine);
        m_connections.emplace_back(newNode.lock().get(), "Trigger", &node, "Dialogue");
    }
}

std::weak_ptr<DialogueWindow::DialogueNode>
DialogueWindow::loadDialogueLineRecursive(
    std::weak_ptr<const quest::DialogueLine> line)
{
    auto foundIt = std::find_if(begin(m_nodes),
                                end(m_nodes), [&line](std::weak_ptr<Node> node)
    {
        auto nodeAsDialogue = std::dynamic_pointer_cast<DialogueNode>(node.lock());
        if (nodeAsDialogue.get())
            return line.lock()->id() == nodeAsDialogue->dialogueLine.lock()->id();
        return false;
    });
    if (foundIt != end(m_nodes))
        return std::static_pointer_cast<DialogueNode>(*foundIt);

    m_nodes.emplace_back(std::make_shared<DialogueNode>());
    auto node = std::static_pointer_cast<DialogueNode>(m_nodes.back());
    node->dialogueLine = line;

    for (size_t i = 0; i < line.lock()->choices().size(); i++)
    {
        std::string l = line.lock()->choices().at(i);
        if (l.empty())
            l = "[auto]";
        node->outputs.push_back({l, 1});
        auto nextLine = line.lock()->getChoice(i);
        if (nextLine.lock())
        {
            auto nextLineNode = loadDialogueLineRecursive(nextLine);

            m_connections.emplace_back(nextLineNode.lock().get(), "Trigger",
                                       node.get(), node->outputs.back().title);
        }


    }
    node->inputs.push_back({"Trigger", 1});

    return node;

}

} // namespace maker::gui
