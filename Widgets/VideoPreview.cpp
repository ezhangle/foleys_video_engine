/*
 ==============================================================================

 Copyright (c) 2019, Foleys Finest Audio - Daniel Walz
 All rights reserved.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE.

 ==============================================================================
 */

namespace foleys
{

VideoPreview::VideoPreview()
{
}

VideoPreview::~VideoPreview()
{
    if (clip)
    {
        clip->removeTimecodeListener (this);
        clip->removeSubtitleListener (this);
    }
}

void VideoPreview::setClip (AVClip* clipToUse)
{
    if (clip)
    {
        clip->removeTimecodeListener (this);
        clip->removeSubtitleListener (this);
    }

    clip = clipToUse;

    if (clip)
    {
        clip->addTimecodeListener (this);
        clip->addSubtitleListener (this);
    }
    repaint();
}

AVClip* VideoPreview::getClip() const
{
    return clip;
}

void VideoPreview::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    if (clip)
    {
        auto image = clip->getCurrentFrame();
        g.drawImage (image, getLocalBounds().toFloat(), placement);
    }

    if (subtitle.isNotEmpty())
    {
        g.drawFittedText (subtitle,
                          getLocalBounds().withTop (getHeight() * 0.9),
                          juce::Justification::centred, 3);
    }
}

void VideoPreview::timecodeChanged (Timecode tc)
{
    if (tc.count > subtitleClear.count)
    {
        subtitle.clear();
        subtitleClear.count = 0;
        currentFrameCount = -1;
    }

    if (currentFrameCount != tc.count)
    {
        currentFrameCount = tc.count;
        repaint();
    }
}

void VideoPreview::setSubtitle (const juce::String& text, Timecode until)
{
    subtitle = text;
    subtitleClear = until;
}

}