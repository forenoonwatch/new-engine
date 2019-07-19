#pragma once

#include "game-render-context.hpp"

class RenderableText : public ECSComponent<RenderableText> {
	public:
		Font* font;
		String text;
		Vector3f color;
		float x = 0.f;
		float y = 0.f;
};

class TextRenderingSystem : public BaseECSSystem {
	public:
		TextRenderingSystem(GameRenderContext& context)
				: context(context) {
			addComponentType(RenderableText::ID);
		}

		virtual void updateComponents(float delta, BaseECSComponent** components) override {
			RenderableText* renderableText = (RenderableText*)components[0];
			context.renderText(*renderableText->font, renderableText->text,
					renderableText->color, renderableText->x, renderableText->y);
		}
	private:
		GameRenderContext& context;
};
