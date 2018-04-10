/* Copyright 2018 QReal Research Group
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

#include <gmock/gmock.h>

#include "semanticTreeTest.h"

using namespace qrTest::robotsTests::SemanticTreeTests;
using namespace qrTest;
using namespace ::testing;


void qrTest::robotsTests::SemanticTreeTests::SemanticTreeTest::SetUp()
{
	const QString empty = "";
	const QStringList pathsToTemplates = {};
	const QString workingFile = "diagrams/oneTest6.qrs";
	const QString mainRepositoryName = "qrm:/ROOT_ID/ROOT_ID/ROOT_ID/ROOT_ID";

	mRepo.reset(new qrRepo::RepoApi(workingFile));
	const qReal::Id repoId = qReal::Id::loadFromString(mainRepositoryName);
	qReal::IdList elements = mRepo->children(repoId);

	for (const qReal::Id element : elements) {
		if (element.element() == "RobotsDiagramNode") {
			qDebug() << element;
		}
	}
	//mRepo->printDebug();
	//qDebug() << mRepo->workingFile();
	mToolbox.reset(new qrtext::lua::LuaToolbox());

	ON_CALL(mModelManager, model()).WillByDefault(ReturnRef(mModel));

	mEditor.reset(new qReal::EditorManager(empty));


	mParserErrorReporter.reset(new utils::ParserErrorReporter(*mToolbox
			, mErrorReporterMock
			, *mEditor
	));

	mProcessor.reset(new generatorBase::lua::LuaProcessor(mErrorReporterMock
			, *mToolbox
			, *mParserErrorReporter));

	mCustomizer.reset(new trik::TrikGeneratorCustomizer(*mRepo
			, mErrorReporterMock
			, mModelManager
			, *mProcessor
			, pathsToTemplates));

	mCustomizer->factory()->initialize();
}

void SemanticTreeTest::loadDiagram(const QString &mainIdName)
{
	const qReal::Id diagramId = qReal::Id::loadFromString(mainIdName);

	mPrimaryControlFlowValidator.reset(new generatorBase::PrimaryControlFlowValidator(*mRepo
			, mErrorReporterMock
			, *mCustomizer
	));

	mReadableControlFlowGenerator.reset(new generatorBase::ReadableControlFlowGenerator(
			*mRepo
			, mErrorReporterMock
			, *mCustomizer
			, *mPrimaryControlFlowValidator.data()
			, diagramId
	));

}

const QString SemanticTreeTest::scheme(const QString &mainIdName)
{
	loadDiagram(mainIdName);
	generatorBase::semantics::SemanticTree *tree = mReadableControlFlowGenerator->generate();
	const QString scheme = tree->treeScheme();
	//qDebug() << scheme;
	delete tree;
	return scheme;
}

TEST_F(SemanticTreeTest, smallSequenceTest) {

	const QString mainIdName = "qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{47bae389-f76d-4510-999b-c8160d1dfc33}";

	EXPECT_TRUE(scheme(mainIdName) == "root@simple;simple;final");
}

TEST_F(SemanticTreeTest, ifTest) {
	const QString mainIdName = "qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{8d5dc72e-c563-409a-9280-a2fc750b3407}";

	EXPECT_TRUE(scheme(mainIdName) == "root@simple;if[simple|simple];final");
}

TEST_F(SemanticTreeTest, forSimpleTest) {
	const QString mainIdName = "qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{a0f7bba3-ca84-4b3b-bbf5-d7d906083dfb}" ;

	EXPECT_TRUE(scheme(mainIdName) == "root@simple;simple;loop[simple];final");
}


TEST_F(SemanticTreeTest, switchSimpleTest) {
	const QString mainIdName = "qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{52582016-18f3-415e-ad5b-c91deef8e42c}";

	EXPECT_TRUE(scheme(mainIdName) == "root@simple;simple;switch[simple|simple|simple];final");
}

TEST_F(SemanticTreeTest, switchWithOneTerminatingBranch) {
	const QString mainIdName = "qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{faeb1b79-2add-48e5-871e-e56da452b8a5}";

	EXPECT_TRUE(scheme(mainIdName) == "root@simple;simple;switch[simple;simple;simple;simple"
				"|simple;simple;simple|simple;if[simple];final];final");
}

TEST_F(SemanticTreeTest, ifWithoutElseBranch) {
	const QString mainIdName = "qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{73f9f970-f1a4-4383-bd29-3cee40dd28bd}";

	EXPECT_TRUE(scheme(mainIdName) == "root@simple;simple;if[simple];final");
}
