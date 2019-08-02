#pragma once

#include "shader.hpp"
#include "vertex-array.hpp"
#include "feedback-buffer.hpp"
#include "render-target.hpp"

class RenderContext {
	public:
		inline RenderContext(RenderDevice& deviceIn, RenderTarget& targetIn)
			: device(&deviceIn)
			, target(&targetIn)
			, clearColor(Color(0, 0, 0)) {}

		inline RenderContext(RenderDevice& deviceIn, RenderTarget& targetIn,
				const Color& clearColorIn)
			: device(&deviceIn)
			, target(&targetIn)
			, clearColor(clearColorIn) {}

		inline void clear(bool shouldClearColor, bool shouldClearDepth,
				bool shouldClearStencil, uint32 stencil);
		inline void clear(bool shouldClearDepth = false);
		inline void draw(Shader& shader, VertexArray& vertexArray, 
				const RenderDevice::DrawParams& drawParams, uint32 numInstances = 1);
		inline void draw(Shader& shader, VertexArray& vertexArray, 
				const RenderDevice::DrawParams& drawParams, uint32 numInstances,
				uint32 numIndices);
		inline void draw(Shader& shader, FeedbackBuffer& feedbackBuffer,
				const RenderDevice::DrawParams& drawParams);

		inline void updateFeedbackBuffer(Shader& shader, FeedbackBuffer& feedbackBuffer);

		inline void setClearColor(const Color& color) { clearColor = color; }
		inline const Color& getClearColor() const { return clearColor; }
	protected:
		inline RenderDevice& getDevice() { return *device; }
	private:
		RenderDevice* device;
		RenderTarget* target;

		Color clearColor;
};

inline void RenderContext::draw(Shader& shader, VertexArray& vertexArray, 
			const RenderDevice::DrawParams& drawParams, uint32 numInstances) {
	device->draw(target->getId(), shader.getId(), vertexArray.getId(),
			drawParams, numInstances, vertexArray.getNumIndices());
}

inline void RenderContext::draw(Shader& shader, VertexArray& vertexArray, 
			const RenderDevice::DrawParams& drawParams, uint32 numInstances,
			uint32 numIndices) {
	device->draw(target->getId(), shader.getId(), vertexArray.getId(),
			drawParams, numInstances, numIndices);
}

inline void RenderContext::draw(Shader& shader, FeedbackBuffer& feedbackBuffer,
		const RenderDevice::DrawParams& drawParams) {
	device->drawFeedbackBuffer(target->getId(), shader.getId(),
			feedbackBuffer.getID(), drawParams);
}

inline void RenderContext::clear(bool shouldClearColor, bool shouldClearDepth,
		bool shouldClearStencil, uint32 stencil) {
	device->clear(target->getId(), shouldClearColor, shouldClearDepth, shouldClearStencil,
			clearColor, stencil);
}

inline void RenderContext::clear(bool shouldClearDepth) {
	device->clear(target->getId(), true, shouldClearDepth, false, clearColor, 0);
}

inline void RenderContext::updateFeedbackBuffer(Shader& shader, FeedbackBuffer& feedbackBuffer) {
	device->updateFeedbackBuffer(shader.getId(), feedbackBuffer.getID());
}
