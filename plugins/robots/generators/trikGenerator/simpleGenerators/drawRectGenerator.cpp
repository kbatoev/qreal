#include "drawRectGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawRectGenerator::DrawRectGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/drawRect.t"
			, {
					Binding::createConverting("@@XCoordinateRect@@", "XCoordinateRect"
							, customizer.factory()->intPropertyConverter(id))
					, Binding::createConverting("@@YCoordinateRect@@", "YCoordinateRect"
							, customizer.factory()->intPropertyConverter(id))
					, Binding::createConverting("@@WidthRect@@", "WidthRect"
							, customizer.factory()->intPropertyConverter(id))
					, Binding::createConverting("@@HeightRect@@", "HeightRect"
							, customizer.factory()->intPropertyConverter(id))
			}
			, parent)
{
}
