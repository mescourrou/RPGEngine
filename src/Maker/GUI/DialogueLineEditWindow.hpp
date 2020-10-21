#pragma once

#include <Window.hpp>
#include <string>
#include <Event.hpp>
#include <ImNodesEz.h>
#include <Stringlist.hpp>

namespace maker::gui
{

class DialogueLineEditWindow : public ImGui::Window
{
  public:
    DialogueLineEditWindow();

    std::string line() const;
    void setLine(const std::string& line);

    void setInputs(const std::vector<ImNodes::Ez::SlotInfo>& inputVector);
    void setOutputs(const std::vector<ImNodes::Ez::SlotInfo>& outputVector);
    void reset();

    ADD_EVENT(SignalLineEdited, std::string);
    ADD_EVENT(SignalInputEdited, std::string, size_t);
    ADD_EVENT(SignalOutputEdited, std::string, size_t);
  protected:
    bool doPrepare() override;

    std::vector<ImNodes::Ez::SlotInfo> m_inputVector;
    stringlist m_inputBuffers;
    std::vector<ImNodes::Ez::SlotInfo> m_outputVector;
    stringlist m_outputBuffers;
    char m_lineBuffer[255];
};

} // namespace maker::gui
