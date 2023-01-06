#include <catch.hpp>
#include "include/Tools/PhysicalPen.hpp"
#include "../include/fakeit.hpp"

TEST_CASE("[PhysicalPen]")
{
	fakeit::Mock<PenUserInterface> penUserMock;
	PhysicalPen physicalPen([&] () -> PenUserInterface&
		{
			return penUserMock.get();
		});

	SECTION("Invokes delete on penDelete")
	{
		fakeit::When(Method(penUserMock, penDelete)).Return();
		physicalPen.penDelete();
		fakeit::Verify(Method(penUserMock, penDelete));
	}

	SECTION("Invokes cancel on penCancel")
	{
		fakeit::When(Method(penUserMock, penDragCancel)).Return();
		physicalPen.penCancel();
		fakeit::Verify(Method(penUserMock, penDragCancel));
	}

	SECTION("Invokes click on down/up")
	{
		fakeit::When(Method(penUserMock, penClicked)).Return();
		physicalPen.penDown();
		physicalPen.penUp();
		fakeit::Verify(Method(penUserMock, penClicked));
	}

	SECTION("Pen drag works (start, update, end)")
	{
		physicalPen.updatePenPosition(sf::Vector2i(10, 10));

		fakeit::When(Method(penUserMock, penDragStarted)).Return();
		physicalPen.penDown();
		physicalPen.updatePenPosition(sf::Vector2i(20, 20));
		fakeit::Verify(Method(penUserMock, penDragStarted));

		fakeit::When(Method(penUserMock, penDragUpdate)).Return();
		physicalPen.updatePenPosition(sf::Vector2i(30, 30));
		fakeit::Verify(Method(penUserMock, penDragUpdate));

		fakeit::When(Method(penUserMock, penDragEnded)).Return();
		physicalPen.penUp();
		fakeit::Verify(Method(penUserMock, penDragEnded));
	}
}
