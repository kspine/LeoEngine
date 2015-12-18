#include <LAssert.h>
#include "Widget.h"
#include "HUDRenderer.h"
#include "HUDUtilities.h"
#include "HUDControl.h"

LEO_BEGIN

HUD_BEGIN

Rect HUDRenderer::Paint(IWidget & wgt, PaintEventArgs && e)
{
	LAssert(&e.GetSender().GetRenderer() == this, "Invalid widget found.");
	if (LB_LIKELY(!e.ClipArea.IsUnStrictlyEmpty()))
		CallEvent<VisualEvent::Paint>(wgt, e);
	return e.ClipArea;
}

ImplDeDtor(HUDPseudoRenderer)

BufferedRenderer::BufferedRenderer(bool b, std::shared_ptr<IImage> p)
	: HUDRenderer(),
	rInvalidated(), pImageBuffer(p), IgnoreBackground(b)
{}

BufferedRenderer::BufferedRenderer(const BufferedRenderer& rd)
	: HUDRenderer(rd),
	rInvalidated(rd.rInvalidated), pImageBuffer(ClonePolymorphic(
		rd.pImageBuffer)), IgnoreBackground(rd.IgnoreBackground)
{}

bool
BufferedRenderer::RequiresRefresh() const
{
	return bool(rInvalidated);
}

void
BufferedRenderer::SetImageBuffer(std::shared_ptr<IImage> p,bool clone)
{
	pImageBuffer = clone ? std::shared_ptr<IImage>(ClonePolymorphic(p)) : p;
}

void
BufferedRenderer::SetSize(const Size& s)
{
	GetImageBuffer().SetSize(s);
	rInvalidated = { {}, s };
}

Rect
BufferedRenderer::CommitInvalidation(const Rect& r)
{
	return rInvalidated |= r;
}

Rect
BufferedRenderer::Paint(IWidget& wgt, PaintEventArgs&& e)
{
	const Rect& r(Validate(wgt, e.GetSender(), e));

	return r;
}

Rect
BufferedRenderer::Validate(IWidget& wgt, IWidget& sender,
	const PaintContext& pc)
{
	LAssert(&sender.GetRenderer() == this, "Invalid widget found.");
	if (RequiresRefresh())
	{
		const Rect& clip(pc.ClipArea & (rInvalidated + pc.Location));

		if (!clip.IsUnStrictlyEmpty())
		{
			if (LB_UNLIKELY(&wgt != &sender)) {
				const auto& g(GetContext());

				//if (!IgnoreBackground && FetchContainerPtr(sender))
				//{
				//	const auto dst(g.GetBufferPtr());
				//	const auto& src(pc.Target);
						//CopyTo(g.GetBufferPtr(), src, g.GetSize(), clip.GetPoint()
							//- pc.Location, clip.GetPoint(), clip.GetSize());
				//}

				PaintEventArgs e(sender,
				{ *g, Point(), (clip - pc.Location) & Rect(g->GetSize()) });

				CallEvent<VisualEvent::Paint>(wgt, e);
				// NOTE: To keep %CommitInvalidation result correct, both
				//	components of the size shall be reset.
				rInvalidated.GetSizeRef() = {};
				return e.ClipArea;
			}
			else {
				PaintEventArgs e(sender,
				{ pc.Target, Point(), (clip - pc.Location) & Rect(pc.Target.GetSize()) });

				CallEvent<VisualEvent::Paint>(wgt, e);
				// NOTE: To keep %CommitInvalidation result correct, both
				//	components of the size shall be reset.
				rInvalidated.GetSizeRef() = {};
				return e.ClipArea;
			}
		}
	}
	return{};
}

HUD_END
LEO_END