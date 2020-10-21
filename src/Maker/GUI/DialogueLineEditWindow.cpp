#include "DialogueLineEditWindow.hpp"

namespace maker::gui
{

DialogueLineEditWindow::DialogueLineEditWindow() :
    ImGui::Window("Line edit"), m_lineBuffer("")
{

}

std::string DialogueLineEditWindow::line() const
{
    return m_lineBuffer;
}

void DialogueLineEditWindow::setLine(const std::string& line)
{
    strcpy(m_lineBuffer, line.data());
}

void DialogueLineEditWindow::setInputs(const std::vector<ImNodes::Ez::SlotInfo>&
                                       inputVector)
{
    m_inputVector = inputVector;
}

void DialogueLineEditWindow::setOutputs(const std::vector<ImNodes::Ez::SlotInfo>&
                                        outputVector)
{
    m_outputVector = outputVector;
}

void DialogueLineEditWindow::reset()
{
    m_lineBuffer[0] = '\0';
}

bool DialogueLineEditWindow::doPrepare()
{
    if (ImGui::InputText("Line", m_lineBuffer, 255))
    {
        getSignalLineEdited().trigger(m_lineBuffer);
    }
    ImGui::TextUnformatted("Inputs");
    for (size_t i = 0; i < m_inputVector.size(); i++)
    {
        if (m_inputBuffers.size() <= i)
            m_inputBuffers.push_back(m_inputVector.at(i).title.c_str(), 255);
        else
            strcpy(m_inputBuffers.get(i), m_inputVector.at(i).title.data());
        ImGui::PushID(i);
        if (ImGui::InputText("", m_inputBuffers.get(i), 255))
        {
            m_inputVector.at(i).title = m_inputBuffers.getStr(i);
            getSignalInputEdited().trigger(m_inputBuffers.getStr(i), i);
        }
        ImGui::PopID();
    }
    ImGui::PushID("InputAdd");
    if (ImGui::Button("Add"))
    {
        m_inputVector.push_back({"", 1});
        getSignalInputEdited().trigger("", m_inputVector.size() - 1);
    }
    ImGui::PopID();

    ImGui::TextUnformatted("Outputs");
    for (size_t i = 0; i < m_outputVector.size(); i++)
    {
        if (m_outputBuffers.size() <= i)
            m_outputBuffers.push_back(m_outputVector.at(i).title.c_str(), 255);
        else
            strcpy(m_outputBuffers.get(i), m_outputVector.at(i).title.data());
        ImGui::PushID(m_inputVector.size() + i);
        if (ImGui::InputText("", m_outputBuffers.get(i), 255))
        {
            m_outputVector.at(i).title = m_outputBuffers.getStr(i);
            getSignalOutputEdited().trigger(m_outputBuffers.getStr(i), i);
        }
        ImGui::PopID();
    }
    ImGui::PushID("OutputAdd");
    if (ImGui::Button("Add"))
    {
        m_outputVector.push_back({"", 1});
        getSignalOutputEdited().trigger("", m_outputVector.size() - 1);
    }
    ImGui::PopID();
    return true;
}

} // namespace maker::gui
