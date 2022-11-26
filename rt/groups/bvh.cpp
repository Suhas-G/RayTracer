#include <rt/groups/bvh.h>
#include <core/point.h>
#include <stack>
#include <tuple>

namespace rt {

    BVH::BVH()
    {
        /* TODO */
        this->splitMethod = SplitMethod::Middle;
    }

    void BVH::rebuildIndex() {
        /* TODO */
        if (primitives.size() == 0) {
            return;
        }

        primitiveInfo = std::vector<BVHPrimitiveInfo>(primitives.size());
        for (int i = 0; i < primitives.size(); i++) {
            primitiveInfo[i].box = primitives[i]->getBounds();
            primitiveInfo[i].primitiveIndex = i;
            // TODO: How to do unbounded boxes?
            rt_assert(!primitiveInfo[i].box.isUnbound());
            primitiveInfo[i].center = 0.5f * (primitiveInfo[i].box.min + primitiveInfo[i].box.max);
        }
        int totalNodes = 0;
        BVHBuildNode* root = recursiveBuild(0, static_cast<int>(primitiveInfo.size()), totalNodes);

        // Swap original primiitives with ordered primitives
        rt_assert(orderedPrimitives.size() == primitives.size());
        primitives.swap(orderedPrimitives);

        orderedPrimitives.clear();
        std::cout << "Primitive count: " << primitives.size() << std::endl;
        std::cout << "Total Node count" << totalNodes << " " << static_cast<int>(root->splitAxis) << std::endl;
        // Create linear nodes
        bvhTree = new LinearBVHNode[totalNodes];
        int offset = 0;
        flattenBHVTree(root, offset);
        
    }

    BVHBuildNode* BVH::recursiveBuild(int start, int end, int& totalNodes) {

        int nPrimitives = end - start;
        if (nPrimitives < 3) {
            return createLeafNode(start, end, totalNodes);
        } else {
            return createInteriorNode(start, end, totalNodes);
        }
    }

    BVHBuildNode* BVH::createInteriorNode(int start, int end, int& totalNodes) {
        totalNodes++;
        BVHBuildNode* node = new BVHBuildNode();

        for (int i = start; i < end; i++) {
            node->box.extend(primitiveInfo[i].box);
        }
        node->splitAxis = node->box.biggestDimensionAxis();
        int splitAxis = static_cast<int>(node->splitAxis);
        float midPoint = 0.5f * (node->box.min[splitAxis] + node->box.max[splitAxis]);

        // Split in the middle approach
        BVHPrimitiveInfo* midPrimitiveInfo = std::partition(&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
            [splitAxis, midPoint](const BVHPrimitiveInfo& pInfo) {
                return (pInfo.center[splitAxis] < midPoint);
            });

        // Integer size is enough for all primitives?
        int mid = static_cast<int>(midPrimitiveInfo - &primitiveInfo[0]);
        // If this happens, ideally I should try next axis?
        // TODO
        if (mid == start || mid == end) {
            // std::cout << "Unable to split" << std::endl;
            mid = (start + end) / 2;
            std::nth_element(&primitiveInfo[start], &primitiveInfo[mid],
                &primitiveInfo[end - 1] + 1,
                [splitAxis](const BVHPrimitiveInfo& a,
                    const BVHPrimitiveInfo& b) {
                        return a.center[splitAxis] < b.center[splitAxis];
                });
        }
        node->initInteriorNode(recursiveBuild(start, mid, totalNodes), recursiveBuild(mid, end, totalNodes));
        return node;
    }

    BVHBuildNode* BVH::createLeafNode(int start, int end, int& totalNodes) {
        totalNodes++;
        // Check if its possible to allocate all build nodes at the same time
        BVHBuildNode* node = new BVHBuildNode();

        int firstPrimOffset = static_cast<int>(orderedPrimitives.size());
        for (int i = start; i < end; i++) {
            orderedPrimitives.push_back(primitives[primitiveInfo[i].primitiveIndex]);
            node->box.extend(primitiveInfo[i].box);
        }
        node->initLeafNode(firstPrimOffset, end - start);
        return node;
    }

    int BVH::flattenBHVTree(BVHBuildNode* buildNode, int& offset) {
        rt_assert(buildNode != nullptr);
        LinearBVHNode* node = &bvhTree[offset];
        node->bounds = buildNode->box;
        int currentOffset = offset;
        offset++;
        rt_assert(offset > currentOffset);
        if (buildNode->nPrimitives > 0) {
            // Leaf node
            node->nPrimitives = static_cast<uint16_t>(buildNode->nPrimitives);
            node->primitiveOffset = buildNode->firstPrimOffset;
        } else {
            // Interior node
            node->axis = static_cast<uint8_t>(buildNode->splitAxis);
            node->nPrimitives = 0;
            flattenBHVTree(buildNode->children[0], offset);
            node->secondChildOffset = flattenBHVTree(buildNode->children[1], offset);
        }
        return currentOffset;
    }

    BBox BVH::getBounds() const {
        BBox box = BBox::empty();
        for (auto p : primitives) {
            box.extend(p->getBounds());
        }
        return box;
    }

    Intersection BVH::intersect(const Ray& ray, float tmin, float tmax) const {
        /* TODO */
        std::stack<int> nodesToVisit;
        Intersection pIntersection = Intersection::failure();
        Intersection finalIntersection = Intersection::failure();
        nodesToVisit.push(0);
        float boundTmin, boundTmax;
        while (!nodesToVisit.empty()) {
            int index = nodesToVisit.top();
            nodesToVisit.pop();
            const LinearBVHNode* node = &bvhTree[index];
            std::tie(boundTmin, boundTmax) = node->bounds.intersect(ray);
            if (boundTmax >= boundTmin) {
                // Have to check inside the node
                if (node->nPrimitives > 0) {
                    // Leaf node
                    for (int i = 0; i < node->nPrimitives; i++) {
                        pIntersection = primitives[node->primitiveOffset + i]->intersect(ray, tmin, tmax);
                        if (pIntersection) {
                            finalIntersection = pIntersection;
                            tmax = finalIntersection.distance;
                        }
                    }
                } else {
                    // Interior node
                    if (ray.dirIsNeg[node->axis]) {
                        nodesToVisit.push(node->secondChildOffset);
                        nodesToVisit.push(index + 1);
                    } else {
                        nodesToVisit.push(index + 1);
                        nodesToVisit.push(node->secondChildOffset);
                    }
                }
            }
        }

        return finalIntersection;
    }

    void BVH::add(Primitive* p) {
        /* TODO */
        this->primitives.push_back(p);
    }

    void BVH::setMaterial(Material* m) {
        /* TODO */
        for (auto p : primitives) {
            p->setMaterial(m);
        }
    }

    void BVH::setCoordMapper(CoordMapper* cm) {
        /* TODO */
        for (auto p : primitives) {
            p->setCoordMapper(cm);
        }
    }

    void BVH::serialize(BVH::Output& output) {
        // To implement this function:
        // - Call output.setNodeCount() with the number of nodes in the BVH
        /* TODO */
        // - Set the root node index using output.setRootId()
        /* TODO */
        // - Write each and every one of the BVH nodes to the output using output.writeNode()
        /* TODO */ NOT_IMPLEMENTED;
    }
    void BVH::deserialize(BVH::Input& input) {
        // To implement this function:
        // - Allocate and initialize input.getNodeCount() nodes
        /* TODO */
        // - Fill your nodes with input.readNode(index)
        /* TODO */
        // - Use the node at index input.getRootId() as the root node
        /* TODO */ NOT_IMPLEMENTED;
    }

    BVH::~BVH() {
        delete[] bvhTree;
    }

    void BVHBuildNode::initLeafNode(int _firstPrimOffset, int _nPrimitives) {
        children[0] = children[1] = nullptr;
        firstPrimOffset = _firstPrimOffset;
        nPrimitives = _nPrimitives;
    }

    void BVHBuildNode::initInteriorNode(BVHBuildNode* leftChild, BVHBuildNode* rightChild) {
        children[0] = leftChild;
        children[1] = rightChild;
        box.extend(leftChild->box);
        box.extend(rightChild->box);
        nPrimitives = 0;
    }

}