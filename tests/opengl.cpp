/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2021 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Application.hpp"
#include "StandaloneWindow.hpp"

#include "../generic/ResizeHandle.hpp"
#include "../opengl/Blendish.cpp"

#include <vector>

START_NAMESPACE_DGL

// --------------------------------------------------------------------------------------------------------------------
// Blendish demo

class BlendishDemo : public BlendishSubWidget
{
    BlendishToolButton toolButton1;
    BlendishToolButton toolButton2;
    BlendishToolButton toolButton3;

    BlendishNumberField numberFieldX;
    BlendishNumberField numberFieldY;
    BlendishNumberField numberFieldW;
    BlendishNumberField numberFieldH;

public:
    BlendishDemo(Widget* const parent)
        : BlendishSubWidget(parent),
          toolButton1(this),
          toolButton2(this),
          toolButton3(this),
          numberFieldX(this),
          numberFieldY(this),
          numberFieldW(this),
          numberFieldH(this)
    {
        toolButton1.setLabel("A toolbutton");
        toolButton2.setLabel("Another toolbutton");
        toolButton3.setLabel("The final toolbutton");
        groupBlendishWidgets(true, &toolButton1, &toolButton2, &toolButton3, nullptr);

        numberFieldX.setAbsoluteY(60);
        numberFieldX.setLabel("X");
        numberFieldY.setLabel("Y");
        numberFieldW.setLabel("Width");
        numberFieldH.setLabel("Height");
        groupBlendishWidgets(false, &numberFieldX, &numberFieldY, &numberFieldW, &numberFieldH, nullptr);
    }
};

// --------------------------------------------------------------------------------------------------------------------
// Main Widgets Demo Window, having a left-side tab-like widget and main area for current widget

class WidgetsDemoWindow : public StandaloneWindow
{
    ResizeHandle resizeHandle;
    std::vector<SubWidget*> widgets;

    template <class W>
    W* createAndAddWidgetOfType(const uint margin)
    {
        W* const w = new W((TopLevelWidget*)this);
        widgets.push_back(w);

        w->setAbsolutePos(margin, margin);
        w->setSize(getWidth()-margin, getHeight()-margin);

        return w;
    }

public:
    WidgetsDemoWindow(Application& app)
        : StandaloneWindow(app),
          resizeHandle(this)
    {
        const double scaleFactor = getScaleFactor();
        const uint margin = (16 + 2) * scaleFactor;

        setSize(480 * scaleFactor, 360 * scaleFactor);
        setTitle("DPF Widgets Demo");

        createAndAddWidgetOfType<BlendishDemo>(margin);
    }

protected:
    void onDisplay() override
    {
        const GraphicsContext& context(getGraphicsContext());
        const double scaleFactor = getScaleFactor();
        const uint margin = 16 * scaleFactor;
        const uint width  = getWidth();
        const uint height = getHeight();

        Color::fromHTML("#5680c2").setFor(context);
        Rectangle<uint>(0, 0, width, margin).draw(context);
        Rectangle<uint>(0, 0, margin, height).draw(context);
        Rectangle<uint>(0, height-margin, width, height).draw(context);
        Rectangle<uint>(width-margin, 0, width, height).draw(context);

        Color::fromHTML("#727272").setFor(context);
        Rectangle<uint>(margin, margin, width-margin*2, height-margin*2).draw(context);
    }

    void onReshape(uint width, uint height) override
    {
        StandaloneWindow::onReshape(width, height);

        for (SubWidget* widget : widgets)
            widget->setSize(width, height);
    }
};

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DGL

int main(int, char**)
{
    USE_NAMESPACE_DGL;

    Application app;
    WidgetsDemoWindow win(app);
    win.show();
    app.exec();

    return 0;
}