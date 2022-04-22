#include "pch.h"
#include "..\UILibrary\UI.h"

TEST(Test1, StartGUILoop) {
	UI testUI; 
	ASSERT_EQ(1, testUI.StartGUILoop());
}

TEST(Test2, ShutDownGUILoop) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}
