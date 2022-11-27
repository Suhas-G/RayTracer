#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <rt/groups/group.h>
#include <rt/bbox.h>

namespace rt {

enum class SplitMethod {
    Middle, SAH
};

struct LinearBVHNode {
    BBox bounds; //24 bytes
    union {
        int primitiveOffset; // 4 bytes
        int secondChildOffset;
    };
    uint16_t nPrimitives; // 2 bytes
    uint8_t axis; // 1 byte
    uint8_t pad[1]; // 1 byte
};

struct BVHPrimitiveInfo {
    BBox box;
    int primitiveIndex;
    Point center;
};

struct BVHBuildNode {
    BBox box = BBox::empty();
    BVHBuildNode *children[2];
    rt::Axis splitAxis;
    int firstPrimOffset = -1;
    int nPrimitives;

    void initLeafNode(int firstPrimOffset, int nPrimitives);
    void initInteriorNode(BVHBuildNode* leftChild, BVHBuildNode* rightChild);
    
};

class BVH : public Group {
public:
    BVH();

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual void rebuildIndex();
    virtual void add(Primitive* p);
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);

    // Do not use this structure as your node layout:
    // It is inefficient and has a large memory footprint.
    struct SerializedNode {
        bool isLeaf;
        BBox bbox;

        // For leaves only
        std::vector<Primitive*> primitives;

        // For internal nodes only
        size_t leftChildId;
        size_t rightChildId;
    };

    // Implement this function if you want to take part in the BVH speed competition
    struct Output {
        virtual void setNodeCount(size_t nodeCount) = 0;
        virtual void setRootId(size_t rootId) = 0;
        virtual void writeNode(size_t nodeId, const SerializedNode& node) = 0;
    };
    void serialize(Output& output);

    // Implementing this is optional and not needed for the BVH speed competition,
    // but it will allow you to run the benchmark utility and test your BVH exporter.
    struct Input {
        virtual size_t getNodeCount() = 0;
        virtual size_t getRootId() = 0;
        virtual const SerializedNode& readNode(size_t nodeId) = 0;
    };
    void deserialize(Input& input);
    ~BVH();
private:
    SplitMethod splitMethod;
    std::vector<BVHPrimitiveInfo> primitiveInfo;
    Primitives orderedPrimitives;
    LinearBVHNode* bvhTree;

    BVHBuildNode* recursiveBuild(int start, int end, int& totalNodes);
    BVHBuildNode* createLeafNode(int start, int end, int& totalNodes);
    BVHBuildNode* createInteriorNode(int start, int end, int& totalNodes);
    int flattenBHVTree(BVHBuildNode* root, int& offset);
};

}

#endif