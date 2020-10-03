#pragma once

#include <Window.hpp>
#include <memory>
#include <Maker.hpp>

namespace maker::gui
{

class DialogueWindow : public ImGui::Window
{
  public:
    explicit DialogueWindow(Maker* maker);

  protected:
    bool doPrepare() override;
  private:
    Maker* m_maker = nullptr;
};


} // namespace maker::gui
