//
// Created by Matt on 8/1/2021.
//

#pragma once
#include <meta/util/testing/TestingComponentViewer.h>

TEST(GraphingPlot, basic_window)
{
    auto viz = std::make_unique<meta::TestingComponentVisualizer>();
    viz->show();
}