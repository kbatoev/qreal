/* Copyright 2018 Konstantin Batoev.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "structuralControlFlowGeneratorTest.h"

#include <gmock/gmock.h>

#include <plugins/robots/generators/generatorBase/src/structuralControlFlowGenerator.h>
#include <../generatorBase/src/structuralControlFlowGenerator.h>
#include <qrrepo/repoApi.h>
#include <qrtest/unitTests/mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/robotModelInterfaceMock.h>
#include <qrtest/unitTests/mocks/qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterMock.h>
#include <mocks/plugins/robots/common/kitBase/include/kitBase/robotModel/robotModelManagerInterfaceMock.h>

#include "../../generators/trik/trikQtsGeneratorLibrary/src/trikQtsMasterGenerator.h"

using namespace qrTest;
using namespace qrTest::robotsTests::structuralControlFlowGeneratorTests;
using namespace ::testing;

void StructuralControlFlowGeneratorTest::SetUp()
{

}

TEST_F(StructuralControlFlowGeneratorTest, firstTest)
{
	EXPECT_TRUE(true);
}


TEST_F(StructuralControlFlowGeneratorTest, secondTest)
{
	const QString workingFile = "tests/for1.qrs";
	qrRepo::RepoApi repo = qrRepo::RepoApi(workingFile, true);
	ErrorReporterMock errorReporterMock;
	mStructuralControlFlowGenerator = new generatorBase::StructuralControlFlowGenerator();

	EXPECT_TRUE(true);
}

