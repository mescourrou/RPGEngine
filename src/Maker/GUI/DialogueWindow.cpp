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
    if (m_dialogueLineEditWindow)
        m_dialogueLineEditWindow->setActive(false);
    m_dialogueLineEditWindow.reset();
    if (ImGui::Button("New dialogue line"))
    {
        m_nodes.emplace_back(std::make_shared<DialogueNode>());
        auto node = std::static_pointer_cast<DialogueNode>(m_nodes.back());
        node->inputs.push_back({"Trigger", 1});
        node->dialogueLine = std::make_shared<quest::DialogueLine>();
    }
    ImNodes::BeginCanvas(&m_canvas);

    updateCharacterNodes();

    for (auto& node : m_nodes)
    {
        displayNode(node);
    }

    for (auto& connection : m_connections)
    {
        ImNodes::Connection(std::get<0>(connection),
                            std::get<1>(connection).c_str(),
                            std::get<2>(connection), std::get<3>(connection).c_str());
    }
    Node* newConnectionInput = nullptr;
    Node* newConnectionOutput = nullptr;
    const char* newConnectionInputTitle = nullptr;
    const char* newConnectionOutputTitle = nullptr;
    int slotKind = 1;

    if (ImNodes::GetNewConnection((void**)&newConnectionInput,
                                  &newConnectionInputTitle,
                                  (void**)&newConnectionOutput, &newConnectionOutputTitle))
    {
        m_connections.push_back({newConnectionInput, newConnectionInputTitle, newConnectionOutput, newConnectionOutputTitle});
    }
    else if (ImNodes::GetPendingConnection((void**)&newConnectionInput,
                                           &newConnectionInputTitle, &slotKind))
    {
        auto findPredicate = [&newConnectionInput,
                              &newConnectionInputTitle](const ConnectionTuple & t)
        {
            if (newConnectionInput->title == std::get<0>(t)->title
                    && !strcmp(newConnectionInputTitle, std::get<1>(t).c_str()))
            {
                return true;
            }
            else if (newConnectionInput->title == std::get<2>(t)->title
                     && !strcmp(newConnectionInputTitle, std::get<3>(t).c_str()))
            {
                return true;
            }
            return false;
        };
        m_connections.remove_if(findPredicate);
    }

    ImNodes::EndCanvas();

    if (m_dialogueLineEditWindow)
        m_dialogueLineEditWindow->prepare();

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
            newNode->title = newNode->characterInformation.name;
        }


    }
}

void DialogueWindow::displayNode(std::shared_ptr<DialogueWindow::Node> node)
{
    if (ImNodes::Ez::BeginNode(node.get(), node->title.c_str(),
                               &node->pos, &node->selected))
    {
        ImNodes::Ez::InputSlots(node->inputs.data(), node->inputs.size());
        ImNodes::Ez::OutputSlots(node->outputs.data(),
                                 node->outputs.size());
        if (node->selected)
        {
            displayNodeInfo(node);
        }
        ImNodes::Ez::EndNode();
    }
}

void DialogueWindow::displayNodeInfo(std::shared_ptr<DialogueWindow::Node> node)
{
    auto nodeAsDialogueLine = std::dynamic_pointer_cast<DialogueNode>(node);
    if (nodeAsDialogueLine)
    {
        if (!m_dialogueLineEditWindow)
            m_dialogueLineEditWindow = std::make_unique<DialogueLineEditWindow>();
        m_dialogueLineEditWindow->setLine(
            nodeAsDialogueLine->dialogueLine->line());

        m_dialogueLineEditWindow->setInputs(nodeAsDialogueLine->inputs);
        m_dialogueLineEditWindow->subscribeSyncToSignalInputEdited([nodeAsDialogueLine](
                    const std::string & input, size_t i)
        {
            if (i < nodeAsDialogueLine->inputs.size())
                nodeAsDialogueLine->inputs.at(i).title = input;
            else
                nodeAsDialogueLine->inputs.push_back({input, 1});
        });

        m_dialogueLineEditWindow->setOutputs(nodeAsDialogueLine->outputs);
        m_dialogueLineEditWindow->subscribeSyncToSignalOutputEdited([nodeAsDialogueLine](
                    const std::string & output, size_t i)
        {
            if (i < nodeAsDialogueLine->outputs.size())
                nodeAsDialogueLine->outputs.at(i).title = output;
            else
                nodeAsDialogueLine->outputs.push_back({output, 1});
        });

        m_dialogueLineEditWindow->subscribeSyncToSignalLineEdited([nodeAsDialogueLine](
                    const std::string & line)
        {
            nodeAsDialogueLine->title = line;
        });
        m_dialogueLineEditWindow->setActive(true);
    }

    m_lastNode = node;

}

void DialogueWindow::loadNPCDialogue(CharacterNode& node)
{
    for (const auto& dialogue : node.characterInformation.dialogueList)
    {
        auto firstLine = dialogue->firstLine();
        auto newNode = loadDialogueLineRecursive(firstLine);
        m_connections.emplace_back(newNode.get(), "Trigger", &node, "Dialogue");
    }
}

std::shared_ptr<DialogueWindow::DialogueNode>
DialogueWindow::loadDialogueLineRecursive(std::shared_ptr<quest::DialogueLine>
        line)
{
    auto foundIt = std::find_if(begin(m_nodes),
                                end(m_nodes), [&line](std::shared_ptr<Node> node)
    {
        auto nodeAsDialogue = std::dynamic_pointer_cast<DialogueNode>(node);
        if (nodeAsDialogue.get())
            return line->id() == nodeAsDialogue->dialogueLine->id();
        return false;
    });
    if (foundIt != end(m_nodes))
        return std::static_pointer_cast<DialogueNode>(*foundIt);

    m_nodes.emplace_back(std::make_shared<DialogueNode>());
    auto node = std::static_pointer_cast<DialogueNode>(m_nodes.back());
    node->dialogueLine = line;

    for (size_t i = 0; i < line->choices().size(); i++)
    {
        std::string l = line->choices().at(i);
        if (l.empty())
            l = "[auto]";
        node->outputs.push_back({l, 1});
        auto nextLine = line->getChoice(i);
        if (nextLine)
        {
            auto nextLineNode = loadDialogueLineRecursive(nextLine);

            m_connections.emplace_back(nextLineNode.get(), "Trigger",
                                       node.get(), node->outputs.back().title);
        }


    }
    node->inputs.push_back({"Trigger", 1});
    node->title = node->dialogueLine->line();

    return node;

}

} // namespace maker::gui
