//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLRenderAPI.hpp"
#include <glad/glad.h>

namespace Voxymore::Core {

	GLenum GetGLPrimitive(DrawMode drawMode) {
		switch (drawMode)  {
			case DrawMode::Points:
			{
				return GL_POINTS;
			}
			break;
			case DrawMode::Lines:
			{
				return GL_LINES;
			}
			break;
			case DrawMode::Triangles:
			{
				return GL_TRIANGLES;
			}
			break;
		}
		VXM_CORE_WARN("The DrawMode {} is unknown. Falling back to Triangles.",(int)drawMode);
		return GL_TRIANGLES;
	}

	void OpenGLRenderAPI::Init() {
		VXM_PROFILE_FUNCTION();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glLineWidth(2);
	}

	void OpenGLRenderAPI::Shutdown() {
		VXM_PROFILE_FUNCTION();
	}

	void OpenGLRenderAPI::SetClearColor(const glm::vec4 &color) {
		VXM_PROFILE_FUNCTION();
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::Clear() {
		VXM_PROFILE_FUNCTION();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRenderAPI::ClearVABinding()
	{
		VXM_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLRenderAPI::ClearShaderBinding()
	{
		VXM_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray> &vertexArray) {
		VXM_PROFILE_FUNCTION();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderAPI::DrawIndexed(DrawMode drawMode, const Ref<VertexArray> &vertexArray) {
		VXM_PROFILE_FUNCTION();
		switch (drawMode) {

			case DrawMode::Points:
				glDrawElements(GL_POINTS, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
				break;
			case DrawMode::Lines:
				glDrawElements(GL_LINES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
				break;
			case DrawMode::Triangles:
			default:
				glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		VXM_PROFILE_FUNCTION();
		glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}

	void OpenGLRenderAPI::UnbindTexture(uint32_t slot)
	{
		VXM_PROFILE_FUNCTION();
		glBindTextureUnit(slot, 0);
	}

	void OpenGLRenderAPI::DrawPatches(uint32_t verticesPerPatch)
	{
		VXM_PROFILE_FUNCTION();
		SetPatchSize(verticesPerPatch);

		glDrawArrays(GL_PATCHES, 0, static_cast<GLsizei>(verticesPerPatch));
	}

	void OpenGLRenderAPI::GPUDraw(uint32_t count, uint32_t offset, DrawMode drawMode) {
		VXM_PROFILE_FUNCTION();
		glDrawArrays(GetGLPrimitive(drawMode), offset, count);
	}

	void OpenGLRenderAPI::SetPatchSize(int32_t verticesPerPatch)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(verticesPerPatch > 0, "Must have a positif number of patches ({0}).", verticesPerPatch);
#ifdef VXM_DEBUG
		GLint maxPatch;
		glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatch);
		VXM_CORE_ASSERT(verticesPerPatch < maxPatch, "Too many patch ({0}), the maximum on this hardware is {1}.", verticesPerPatch, maxPatch);
#endif
		glPatchParameteri(GL_PATCH_VERTICES, verticesPerPatch);
	}

	void OpenGLRenderAPI::DisableDepth() {
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRenderAPI::EnableDepth() {
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRenderAPI::EnableWireframe() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void OpenGLRenderAPI::DisableWireframe() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	void OpenGLRenderAPI::EnableDoubleSided()
	{
		glEnable(GL_CULL_FACE);
	}
	void OpenGLRenderAPI::DisableDoubleSided()
	{
		glDisable(GL_CULL_FACE);
	}

	void OpenGLRenderAPI::CopyColorBuffer(Ref<Framebuffer> source, uint32_t sourceColorAttachmentIndex, Ref<Framebuffer> target, uint32_t targetColorAttachmentIndex) {
		const auto& srcSpec = source->GetSpecification();
		const auto& trgSpec = target->GetSpecification();

		VXM_CORE_CHECK(srcSpec.Width == trgSpec.Width, "The Width of the framebuffer aren't the same.");
		VXM_CORE_CHECK(srcSpec.Height == trgSpec.Height, "The Height of the framebuffer aren't the same.");

		// bind the source framebuffer and select a color attachment to copy from
		glNamedFramebufferReadBuffer(source->GetRendererID(), GL_COLOR_ATTACHMENT0 + sourceColorAttachmentIndex);
		glNamedFramebufferDrawBuffer(target->GetRendererID(), GL_COLOR_ATTACHMENT0 + targetColorAttachmentIndex);

		// copy
		glBlitFramebuffer(0,0,srcSpec.Width, srcSpec.Height, 0,0,trgSpec.Width,trgSpec.Height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	void OpenGLRenderAPI::CopyDepthBuffer(Ref<Framebuffer> source, Ref<Framebuffer> target) {
		const auto& srcSpec = source->GetSpecification();
		const auto& trgSpec = target->GetSpecification();

		VXM_CORE_CHECK(srcSpec.Width == trgSpec.Width, "The Width of the framebuffer aren't the same.");
		VXM_CORE_CHECK(srcSpec.Height == trgSpec.Height, "The Height of the framebuffer aren't the same.");

		// bind the source framebuffer and select a color attachment to copy from
		glBlitNamedFramebuffer(source->GetRendererID(),target->GetRendererID(),0,0,srcSpec.Width, srcSpec.Height, 0,0,trgSpec.Width,trgSpec.Height, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	}
	void OpenGLRenderAPI::SetupDeferredRender(uint32_t positionTexID, uint32_t normalTexID, uint32_t texCoordTexID, uint32_t colorTexID, uint32_t entityIdTexID, uint32_t depthTexID) {
		glBindTextureUnit(0, positionTexID);
		glBindTextureUnit(1, normalTexID);
		glBindTextureUnit(2, texCoordTexID);
		glBindTextureUnit(3, colorTexID);
		glBindTextureUnit(4, entityIdTexID);
	}

	void OpenGLRenderAPI::DrawCubemap(Ref<Cubemap> cubemap, Ref<Shader> shader, Ref<VertexArray> mesh)
	{
		VXM_PROFILE_FUNCTION();
		glDepthMask(GL_FALSE);
		shader->Bind();
		cubemap->Bind();
		mesh->Bind();
		DrawIndexed(mesh);
		glDepthMask(GL_TRUE);
	}

} // Voxymore::Core