/* Copyright 2017 QReal Research Group
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
#include "structuralControlFlowGenerator.h"

using namespace qReal;
using namespace generatorBase;
using namespace semantics;


StructuralControlFlowGenerator::StructuralControlFlowGenerator(const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, PrimaryControlFlowValidator &validator
		, const Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
	, mNumberOfVerteces(0)
{
}

ControlFlowGeneratorBase *StructuralControlFlowGenerator::cloneFor(const Id &diagramId, bool cloneForNewDiagram)
{
	// to do
	return nullptr;
}

void StructuralControlFlowGenerator::beforeSearch()
{
	// to do
}

void StructuralControlFlowGenerator::visitFinal(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitFork(const Id &id, QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitJoin(const Id &id, QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitRegular(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitConditional(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitLoop(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitSwitch(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitUnknown(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::afterSearch()
{
	// to do
}

void StructuralControlFlowGenerator::performGeneration()
{
	ControlFlowGeneratorBase::performGeneration();
	// to check whether diagram was right

	calculatePredecessors();
	findDominators();

	QSet<int> currentNodesInGraph;
	for (int i = 0; i < mNumberOfVerteces; ++i) {
		currentNodesInGraph.insert(i);
	}

	performAnalysis(currentNodesInGraph);
}

void StructuralControlFlowGenerator::buildGraph(const Id &id, const QList<LinkInfo> &links)
{
	int numberOfCurrentVertex = -1;
	if (mNumbersOfElements.contains(id)) {
		numberOfCurrentVertex = mNumbersOfElements[id];
	} else {
		numberOfCurrentVertex = mNumberOfVerteces;
		mNumberOfVerteces++;
		mNumbersOfElements[id] = numberOfCurrentVertex;
	}

	for (const LinkInfo link : links) {
		int targetNumber = -1;
		if (mNumbersOfElements.contains(link.target)) {
			targetNumber = mNumbersOfElements[link.target];
		} else {
			targetNumber = mNumberOfVerteces;
			mNumberOfVerteces++;
			mNumbersOfElements[link.target] = targetNumber;
		}
		mGraph[numberOfCurrentVertex].push_back(targetNumber);
	}
}

void StructuralControlFlowGenerator::calculatePredecessors()
{
	const int root = 0;
	mPredecessors[root].clear();

	for (int i = 0; i < mNumberOfVerteces; i++) {
		for (int t = 0; t < mGraph[i].size(); t++) {
			int destination = mGraph[i].at(t);
			mPredecessors[destination].push_back(i);
		}
	}
}

// to do
// to save invariant that mGraph only has valid nodes and edges,
// so it would be possible not to pass setOfNodes
void StructuralControlFlowGenerator::performAnalysis(QSet<int> &restNodes)
{
	for (int i = 0; i < mNumberOfVerteces; i++) {
		mUsed[i] = false;
		mPostOrder[i] = -1;
	}
	int time = 0;
	dfs(0, time);

	int currentTime = 0;
	int maxTime = time;
	while (restNodes.size() > 1 && currentTime < maxTime) {
		int currentNode = -1;

		// dummy cycle for finding node number that
		// has postOrder equal to currentTime
		for (int i = 0; i < maxNumberOfVerteces; ++i) {
			if (mPostOrder[i] == currentTime) {
				currentNode = i;
				break;
			}
		}


	}
}

void StructuralControlFlowGenerator::findDominators()
{
	const int root = 0;
	QSet<int> allVerteces;

	for (int i = 0; i < mNumberOfVerteces; i++) {
		allVerteces.insert(i);
	}

	mDominators[root].insert(root);

	for (int i = 1; i < mNumberOfVerteces; i++) {
		mDominators[i] = allVerteces;
	}

	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;
		QSet<int> newDominators = allVerteces;
		for (int i = 1; i < mNumberOfVerteces; i++) {
			for (int t = 0; t < mPredecessors[i].size(); t++) {
				int predecessor = mPredecessors[i].at(t);
				newDominators.intersect(mDominators[predecessor]);
			}
			// adding the current number, because reflexivity of dominance relation
			newDominators.insert(i);
			if (newDominators != mDominators[i]) {
				somethingChanged = true;
				mDominators[i] = newDominators;
			}
		}
	}

	// for debugging
	for (int i = 0; i < mNumberOfVerteces; i++) {
		qDebug() << i << " : " << mDominators[i] << endl;
	}
}

void StructuralControlFlowGenerator::dfs(int u, int &postOrderLabel)
{
	mUsed[u] = true;
	for (int t = 0; t < mGraph[u].size(); t++) {
		int v = mGraph[u].at(t);
		if (!mUsed[v]) {
			dfs(v, postOrderLabel);
		}
	}
	mPostOrder[u] = postOrderLabel;
	postOrderLabel++;
}

StructuralControlFlowGenerator::RegionType StructuralControlFlowGenerator::acyclicRegionType(QSet<int> restNodes, int &nodeNumber, QSet<int> &nodesThatComposeRegion)
{
	nodesThatComposeRegion.clear();

	int currentNodeNumber = nodeNumber;
	bool hasOnlyOneIncomingEdge = true;
	bool hasOnlyOneOutcomingEdge = mGraph[currentNodeNumber].size() == 1;

	while (hasOnlyOneIncomingEdge && hasOnlyOneOutcomingEdge) {
		nodesThatComposeRegion.insert(currentNodeNumber);
		currentNodeNumber = mGraph[currentNodeNumber].at(0);
		hasOnlyOneIncomingEdge = mPredecessors[currentNodeNumber].size() == 1;
		hasOnlyOneOutcomingEdge = mGraph[currentNodeNumber].size() == 1;
	}

	if (hasOnlyOneIncomingEdge) {
		nodesThatComposeRegion.insert(currentNodeNumber);
	}

	currentNodeNumber = nodeNumber;
	hasOnlyOneIncomingEdge = mPredecessors[currentNodeNumber].size() == 1;
	hasOnlyOneOutcomingEdge = true;

	while (hasOnlyOneIncomingEdge && hasOnlyOneOutcomingEdge) {
		nodesThatComposeRegion.insert(currentNodeNumber);
		currentNodeNumber = mPredecessors[currentNodeNumber].at(0);
		hasOnlyOneIncomingEdge = mPredecessors[currentNodeNumber].size() == 1;
		hasOnlyOneOutcomingEdge = mGraph[currentNodeNumber].size() == 1;
	}

	if (hasOnlyOneOutcomingEdge) {
		nodesThatComposeRegion.insert(currentNodeNumber);
	}

	nodeNumber = currentNodeNumber;

	if (nodesThatComposeRegion.size() == 2) {
		return Block;
	}

	// checking for IfThenElse
	if (mGraph[nodeNumber].size() == 2) {
		int m = mGraph[nodeNumber].at(0);
		int n = mGraph[nodeNumber].at(1);

		if (mGraph[m].size() == 1 && mGraph[n].size() == 1
				&& mGraph[m].at(0) == mGraph[n].at(0)
				&& mPredecessors[m].size() == 1 && mPredecessors[n].size() == 1) {
			nodesThatComposeRegion.clear();
			nodesThatComposeRegion.insert(nodeNumber);
			nodesThatComposeRegion.insert(m);
			nodesThatComposeRegion.insert(n);
			return IfThenElse;
		}

	}

}


StructuralControlFlowGenerator::Node::Node(StructuralControlFlowGenerator::RegionType type, int number)
	: mRegionType(type)
	, mNumber(number)
	, mParent(nullptr)
{
}

void StructuralControlFlowGenerator::Node::appendChild(StructuralControlFlowGenerator::Node *child)
{
	mChildren.append(child);
}

void StructuralControlFlowGenerator::Node::appendChildren(const QVector<StructuralControlFlowGenerator::Node *> &children)
{
	for (StructuralControlFlowGenerator::Node *child : children) {
		mChildren.append(child);
	}
}

StructuralControlFlowGenerator::Node *StructuralControlFlowGenerator::Node::structOf() const
{
	return mParent;
}

void StructuralControlFlowGenerator::Node::setParent(StructuralControlFlowGenerator::Node *parent)
{
	mParent = parent;
}

StructuralControlFlowGenerator::RegionType StructuralControlFlowGenerator::Node::structType() const
{
	return mRegionType;
}

QVector<StructuralControlFlowGenerator::Node *> StructuralControlFlowGenerator::Node::structNodes() const
{
	return mChildren;
}

