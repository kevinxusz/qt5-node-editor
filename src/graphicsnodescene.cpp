/* See LICENSE file for copyright and license details. */

#include "graphicsnodescene.hpp"
#include <cmath>
#include <QPainter>
#include <QGraphicsTextItem>
#include <algorithm>
#include <iostream>

// TODO: move to graphicsnodeview. use graphicsnodescene for management

GraphicsNodeScene::GraphicsNodeScene(QObject *parent)
: QGraphicsScene(parent)
, _color_background(QColor("#393939"))
, _color_light(QColor("#2F2F2F"))
, _color_dark(QColor("#292929"))
, _pen_light(QPen(_color_light))
, _pen_dark(QPen(_color_dark))
, _brush_background(_color_background)
{
	// initialize default pen settings
	for (auto p : {&_pen_light, &_pen_dark}) {
		p->setWidth(0);
	}

	// initialize the background
	setBackgroundBrush(_brush_background);
}


/*
 * TODO: move the visualization into the graphicsview, and move all the GUI
 * logic into the graphicsnodescene
 */
void GraphicsNodeScene::
drawBackground(QPainter *painter, const QRectF &rect)
{
	// call parent method
	QGraphicsScene::drawBackground(painter, rect);

	// augment the painted with grid
	const int gridsize = 20;
	auto left = static_cast<int>(std::floor(rect.left()));
	auto right = static_cast<int>(std::ceil(rect.right()));
	auto top = static_cast<int>(std::floor(rect.top()));
	auto bottom = static_cast<int>(std::ceil(rect.bottom()));

	// compute indices of lines to draw
	const auto first_left = left - (left % gridsize);
	const auto first_top = top - (top % gridsize);

	// compute lines to draw and
	std::vector<QLine> lines_light;
	std::vector<QLine> lines_dark;
	for (auto x = first_left; x <= right; x += gridsize) {
		if (x % 100 != 0)
			lines_light.push_back(QLine(x, top, x, bottom));
		else
			lines_dark.push_back(QLine(x, top, x, bottom));
	}
	for (auto y = first_top; y <= bottom; y += gridsize) {
		if (y % 100 != 0)
			lines_light.push_back(QLine(left, y, right, y));
		else
			lines_dark.push_back(QLine(left, y, right, y));
	}

	// draw calls
	painter->setPen(_pen_light);
	painter->drawLines(lines_light.data(), lines_light.size());

	painter->setPen(_pen_dark);
	painter->drawLines(lines_dark.data(), lines_dark.size());
}

