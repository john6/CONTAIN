/*
Test class notes:
There is a big problem with the linker that I was not able to solve,
Object files for the CONTAIN project are not being generated in the test project.
No amount of messing with the config and including more directories was helping. '
As a work-around I've included all the CONTAIN projet's cpp files in this test file.

I have no idea how this test class is supposed to store attributes.
It seems like I'm the only person in the world that's using Visual Studio test classes because I can't find anything online about them.
Every single test method is going to have to redefine everything from scratch because when I leave them as Class attributes I get permission errors.
*/


#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "CppUnitTest.h"
#include "Transform.h"
#include "Transform.cpp"
#include "MassData.h"
#include "MassData.cpp"
#include "Shape.h"
#include "Shape.cpp"
#include "Circle.h"
#include "Circle.cpp"
#include "Rectangle.h"
#include "Rectangle.cpp"
#include "Entity.h"
#include "Entity.cpp"
#include "CollisionData.h"
#include "CollisionData.cpp"
#include "Physics.h"
#include "Physics.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{

	public:

		//TEST_METHOD(CircleCollisionTest1)
		//{
		//	std::shared_ptr<Circle> testCircle1 = std::make_shared<Circle>(60.0f);
		//	std::shared_ptr<Circle> testCircle2 = std::make_shared<Circle>(60.0f);
		//	Entity testCircleEnt1 = Entity(testCircle1);
		//	Entity testCircleEnt2 = Entity(testCircle2);
		//	testCircleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testCircleEnt2.GetTransform()->SetPosition(Vector2f(0, 0));
		//	//Assert::IsTrue(Physics::TestCollision(&testCircleEnt1, &testCircleEnt2));
		//}

		//TEST_METHOD(CircleCollisionTest2)
		//{
		//	std::shared_ptr<Circle> testCircle1 = std::make_shared<Circle>(60.0f);
		//	std::shared_ptr<Circle> testCircle2 = std::make_shared<Circle>(60.0f);
		//	Entity testCircleEnt1 = Entity(testCircle1);
		//	Entity testCircleEnt2 = Entity(testCircle2);
		//	testCircleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testCircleEnt2.GetTransform()->SetPosition(Vector2f(80, 0));
		//	//Assert::IsTrue(Physics::TestCollision(&testCircleEnt1, &testCircleEnt2));
		//}

		//TEST_METHOD(CircleCollisionTest3)
		//{
		//	std::shared_ptr<Circle> testCircle1 = std::make_shared<Circle>(60.0f);
		//	std::shared_ptr<Circle> testCircle2 = std::make_shared<Circle>(60.0f);
		//	Entity testCircleEnt1 = Entity(testCircle1);
		//	Entity testCircleEnt2 = Entity(testCircle2);
		//	testCircleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testCircleEnt2.GetTransform()->SetPosition(Vector2f(0, 100));
		//	//Assert::IsTrue(Physics::TestCollision(&testCircleEnt1, &testCircleEnt2));
		//}

		//TEST_METHOD(CircleCollisionTest4)
		//{
		//	std::shared_ptr<Circle> testCircle1 = std::make_shared<Circle>(60.0f);
		//	std::shared_ptr<Circle> testCircle2 = std::make_shared<Circle>(60.0f);
		//	Entity testCircleEnt1 = Entity(testCircle1);
		//	Entity testCircleEnt2 = Entity(testCircle2);
		//	testCircleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testCircleEnt2.GetTransform()->SetPosition(Vector2f(300, 0));
		//	//Assert::IsFalse(Physics::TestCollision(&testCircleEnt1, &testCircleEnt2));
		//}

		//TEST_METHOD(CircleCollisionTest5)
		//{
		//	std::shared_ptr<Circle> testCircle1 = std::make_shared<Circle>(60.0f);
		//	std::shared_ptr<Circle> testCircle2 = std::make_shared<Circle>(60.0f);
		//	Entity testCircleEnt1 = Entity(testCircle1);
		//	Entity testCircleEnt2 = Entity(testCircle2);
		//	testCircleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testCircleEnt2.GetTransform()->SetPosition(Vector2f(0, 300));
		//	//Assert::IsFalse(Physics::TestCollision(&testCircleEnt1, &testCircleEnt2));
		//}

		//TEST_METHOD(CircleCollisionTest6)
		//{
		//	std::shared_ptr<Circle> testCircle1 = std::make_shared<Circle>(60.0f);
		//	std::shared_ptr<Circle> testCircle2 = std::make_shared<Circle>(60.0f);
		//	Entity testCircleEnt1 = Entity(testCircle1);
		//	Entity testCircleEnt2 = Entity(testCircle2);
		//	testCircleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testCircleEnt2.GetTransform()->SetPosition(Vector2f(100, 100));
		//	//Assert::IsFalse(Physics::TestCollision(&testCircleEnt1, &testCircleEnt2));
		//}

		//TEST_METHOD(RectangleCollisionTest1)
		//{
		//	std::shared_ptr<Rectangle> testRect1 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	std::shared_ptr<Rectangle> testRect2 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testRect1);
		//	Entity testRectangleEnt2 = Entity(testRect2);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(0, 0));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(RectangleCollisionTest2)
		//{
		//	std::shared_ptr<Rectangle> testRect1 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	std::shared_ptr<Rectangle> testRect2 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testRect1);
		//	Entity testRectangleEnt2 = Entity(testRect2);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(30, 30));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(0, 0));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(RectangleCollisionTest3)
		//{
		//	std::shared_ptr<Rectangle> testRect1 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	std::shared_ptr<Rectangle> testRect2 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testRect1);
		//	Entity testRectangleEnt2 = Entity(testRect2);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(50, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(0, 50));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(RectangleCollisionTest4)
		//{
		//	std::shared_ptr<Rectangle> testRect1 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	std::shared_ptr<Rectangle> testRect2 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testRect1);
		//	Entity testRectangleEnt2 = Entity(testRect2);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(300, 0));
		//	//Assert::IsFalse(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(RectangleCollisionTest5)
		//{
		//	std::shared_ptr<Rectangle> testRect1 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	std::shared_ptr<Rectangle> testRect2 = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testRect1);
		//	Entity testRectangleEnt2 = Entity(testRect2);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(0, 300));
		//	//Assert::IsFalse(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(CircleRectCollisionTest1)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(0, 0));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(CircleRectCollisionTest2)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 25));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(25, 0));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(CircleRectCollisionTest3)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(50, 50));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(CircleRectCollisionTest4)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(200, 200));
		//	//Assert::IsFalse(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(CircleRectCollisionTest5)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(200, 200));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(400, 400));
		//	//Assert::IsFalse(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(CircleRectCollisionTest6)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(400, 400));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(0, 0));
		//	//Assert::IsFalse(Physics::TestCollision(&testRectangleEnt1, &testRectangleEnt2));
		//}

		//TEST_METHOD(RectCircleCollisionTest1)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(0, 0));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt2, &testRectangleEnt1));
		//}

		//TEST_METHOD(RectCircleCollisionTest2)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 25));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(25, 0));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt2, &testRectangleEnt1));
		//}

		//TEST_METHOD(RectCircleCollisionTest3)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(50, 50));
		//	//Assert::IsTrue(Physics::TestCollision(&testRectangleEnt2, &testRectangleEnt1));
		//}

		//TEST_METHOD(RectCircleCollisionTest4)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(0, 0));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(200, 200));
		//	//Assert::IsFalse(Physics::TestCollision(&testRectangleEnt2, &testRectangleEnt1));
		//}

		//TEST_METHOD(RectCircleCollisionTest5)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(200, 200));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(400, 400));
		//	//Assert::IsFalse(Physics::TestCollision(&testRectangleEnt2, &testRectangleEnt1));
		//}

		//TEST_METHOD(RectCircleCollisionTest6)
		//{
		//	std::shared_ptr<Circle> testCircle = std::make_shared<Circle>(50.0f);
		//	std::shared_ptr<Rectangle> testRect = std::make_shared<Rectangle>(50.0f, 50.0f);
		//	Entity testRectangleEnt1 = Entity(testCircle);
		//	Entity testRectangleEnt2 = Entity(testRect);
		//	testRectangleEnt1.GetTransform()->SetPosition(Vector2f(400, 400));
		//	testRectangleEnt2.GetTransform()->SetPosition(Vector2f(0, 0));
		//	//Assert::IsFalse(Physics::TestCollision(&testRectangleEnt2, &testRectangleEnt1));
		//}

	};
}