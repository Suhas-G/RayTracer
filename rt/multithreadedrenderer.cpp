#include <core/scalar.h>
#include <core/image.h>
#include <core/random.h>
#include <rt/multithreadedrenderer.h>
#include <rt/ray.h>
#include <rt/cameras/camera.h>
#include <rt/integrators/integrator.h>
#include <iostream>
#include <thread>

namespace rt {

    void executeThread(Camera *cam, Integrator* integrator, 
        std::vector<RGBColor>& results, 
        std::vector<std::tuple<float, float, rt::uint, rt::uint>>& pixelsToSample, 
        uint64_t startIndex, uint64_t endIndex, uint64_t tid) {
            // rt::setRandomSeed(tid * 1000);
            float sscx, sscy;
            rt::uint x, y;
            for (uint64_t i = startIndex; i < endIndex; i++) {
                std::tie(sscx, sscy, x, y) = pixelsToSample[i];
                Ray ray = cam->getPrimaryRay(sscx, sscy);
                results[i] = integrator->getRadiance(ray);
            }
        }

    MultiThreadedRenderer::MultiThreadedRenderer(Camera* cam, Integrator* integrator)
        : cam(cam), integrator(integrator), samples(1)
    {}


    void MultiThreadedRenderer::renderWithMultipleSamples(Image& img) {
        const auto width = img.width();
        const auto height = img.height();
        float ndcx, ndcy, sscx, sscy;

        rt::uint verticalSamples = static_cast<rt::uint>(std::floor(std::sqrt(samples)));
        rt::uint horizontalSamples = verticalSamples + ((samples - (verticalSamples * verticalSamples)) / verticalSamples);
        float sampleHeight = 1.0f / verticalSamples;
        float sampleWidth = 1.0f / horizontalSamples;
        float colorWeight = (1.0f / (verticalSamples * horizontalSamples));

        std::vector<std::tuple<float, float, rt::uint, rt::uint>> pixelsToSample = std::vector<std::tuple<float, float, rt::uint, rt::uint>>(verticalSamples * horizontalSamples * width * height);
        std::vector<RGBColor> colors = std::vector<RGBColor>(verticalSamples * horizontalSamples * width * height);
        uint64_t noOfThreads = std::thread::hardware_concurrency();
        std::cout << "No of threads: " << noOfThreads << std::endl;
        std::vector<std::thread> threads;
        uint64_t count = 0;

        for (rt::uint x = 0; x < width; x++) {
            for (rt::uint y = 0; y < height; y++) {
                img(x, height - y - 1) = RGBColor::rep(0.0f);
                for (rt::uint i = 0; i < horizontalSamples; i++) {
                    for (rt::uint j = 0; j < verticalSamples; j++) {
                        float xOffset = (i * sampleWidth) + (rt::random() * sampleWidth);
                        float yOffset = (j * sampleHeight) + (rt::random() * sampleHeight);
                        ndcx = (x + xOffset) / width;
                        ndcy = (y + yOffset) / height;
                        sscx = (ndcx * 2) - 1;
                        sscy = (ndcy * 2) - 1;
                        pixelsToSample[count] = std::make_tuple(sscx, sscy, x, y);
                        count++;
                    }
                }

            }
        }

        uint64_t samplesPerThread = std::ceil((double)count / (double)noOfThreads);
        uint64_t startIndex, endIndex = 0;
        for (uint64_t tid = 0; tid < noOfThreads; tid++) {
            startIndex = tid * samplesPerThread;
            endIndex = std::min(startIndex + samplesPerThread, count);
            threads.emplace_back(executeThread, cam, integrator, std::ref(colors), std::ref(pixelsToSample),
                startIndex, endIndex, tid);
        }



        for (auto& t: threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        
        
        rt::uint x, y;
        for (uint64_t i = 0; i < pixelsToSample.size(); i++) {
            std::tie(sscx, sscy, x, y) = pixelsToSample[i];
            img(x, height - y - 1) = img(x, height - y - 1) + (colorWeight * colors[i]);
        }
        threads.clear();

    }


    void MultiThreadedRenderer::render(Image& img) {
        /* TODO */
        const auto width = img.width();
        const auto height = img.height();
        float ndcx, ndcy, sscx, sscy;

        if (samples == 1) {
            for (rt::uint x = 0; x < width; x++) {
                for (rt::uint y = 0; y < height; y++) {
                    ndcx = (x + 0.5f) / width;
                    ndcy = (y + 0.5f) / height;
                    sscx = (ndcx * 2) - 1;
                    sscy = (ndcy * 2) - 1;
                    Ray ray = cam->getPrimaryRay(sscx, sscy);
                    img(x, height - y - 1, integrator->getRadiance(ray));
                }
            }
        } else {
            try {
                renderWithMultipleSamples(img);
            } catch (std::exception& e) {
            std::cout << "Hehehe" << std::endl;
        }
        }


    }

}