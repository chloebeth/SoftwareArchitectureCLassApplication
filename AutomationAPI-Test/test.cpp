#include "pch.h"
#include "..\AutomationBinding\AutomationAPI_BlockBuilder.h"

TEST(TestHeight, TestHeightBlockBuilder) {
	AutomationAPI::BlockBuilder* builder;
	EXPECT_EQ(0, builder->GetHeight());
}

TEST(TestWidth, TestWidthBlockBuilder) {
	AutomationAPI::BlockBuilder* builder;
	EXPECT_EQ(0, builder->GetWidth());
}
