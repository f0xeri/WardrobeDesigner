#if !defined(CLUSTER_HH)
#define CLUSTER_HH

#include <memory>
#include <stack>
#include <vector>
#include <tuple>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Tools.hpp"


class Cluster
{
private:
    glm::vec2 lower;
    glm::vec2 upper;
    Cluster *firstChild;
    Cluster *secondChild;
    Cluster *parent;

public:

    bool is_root()
    {
        return parent == nullptr;
    }

    bool is_leaf()
    {
        return firstChild == nullptr;
    }

    bool is_vertical()
    {
        return FLOAT_EQUAL(firstChild->lower.y,secondChild->lower.y);
    }

    Cluster(glm::vec2 lower, glm::vec2 upper, Cluster *parent = nullptr)
    {
        firstChild = nullptr;
        secondChild = nullptr;
        this->parent = parent;
        this->lower = lower;
        this->upper = upper;
    }

    void split_test(float separator_offset, float separator_width, bool isVertical)
    {
        if (isVertical)
        {
            firstChild = new Cluster(lower, glm::vec2(lower.x + separator_offset, upper.y), this);
            secondChild = new Cluster(lower + glm::vec2(separator_offset + separator_width, 0), upper, this);
        }
        else
        {
            firstChild = new Cluster(lower, glm::vec2(upper.x, lower.y + separator_offset), this);
            secondChild = new Cluster(lower + glm::vec2(0, separator_offset + separator_width), upper, this);
        }
    }

    void split(glm::vec2 separator_pos, float separator_width, bool isVertical)
    {
        if (isVertical)
        {
            firstChild = new Cluster(lower, glm::vec2(separator_pos.x , upper.y), this);
            secondChild = new Cluster(glm::vec2(separator_pos.x + separator_width, lower.y), upper, this);
        }
        else
        {
            firstChild = new Cluster(lower, glm::vec2(upper.x, separator_pos.y ), this);
            secondChild = new Cluster(glm::vec2(lower.x, separator_pos.y + separator_width), upper, this);
        }
    }

    Cluster *toLocalCluster(glm::vec2 pos)
    {
        if (is_leaf())
        {
            return this;
        }

        int target = !is_vertical();

        if (pos[target] < firstChild->upper[target])
        {
            return firstChild->toLocalCluster(pos);
        }

        if (pos[target] > secondChild->lower[target])
        {
            return secondChild->toLocalCluster(pos);
        }

        return nullptr;
    }

    ~Cluster(){

    };

    friend class ClusterManager;
};

class ClusterManager
{
private:
    Cluster *root;
    std::vector<Cluster *> clusters;
    glm::vec3 origin;
    float depth;

public:
    glm::vec3 getPos(Cluster *c)
    {
        if(c->is_vertical())
            return glm::vec3(c->firstChild->upper.x, c->firstChild->lower.y,-depth);
        return glm::vec3(c->firstChild->lower.x, c->firstChild->upper.y,-depth);
    }

    glm::vec3 getScale(Cluster *c)
    {
        if (c->is_vertical())
        {
            return glm::vec3((c->firstChild->upper.x - c->secondChild->lower.x) * -1, c->upper.y - c->lower.y, depth);
        }
        else
        {
            return glm::vec3(c->upper.x - c->lower.x, (c->firstChild->upper.y - c->secondChild->lower.y) * -1, depth);
        }
    }

    ClusterManager(glm::vec3 size, glm::vec3 origin) : origin(origin)
    {
        glm::vec2 size2d = glm::vec2(size.x, size.y);
        glm::vec2 origin2d = glm::vec2(origin.x, origin.y);
        depth = size.z;
        root = new Cluster(origin2d, origin2d + size2d);
        clusters.push_back(root);
    }

    Cluster *trySplit(glm::vec3 pos, float width, bool isVertical)
    {
        Cluster *c = root->toLocalCluster(glm::vec2(pos.x, pos.y));
        if (c)
        {
            c->split(glm::vec2(pos.x, pos.y), width, isVertical);
            clusters.push_back(c->firstChild);
            clusters.push_back(c->secondChild);
        }
        return c;
    }

    void tryMoveSeparator(Cluster *c, float delta)
    {
        int target = !c->is_vertical();
        std::vector<Cluster *> upper_clusters;
        std::vector<Cluster *> lower_clusters;

        float thiccness = c->secondChild->lower[target] - c->firstChild->upper[target] ;
        for (Cluster *cluster : clusters)
        {
            if (FLOAT_EQUAL(cluster->lower[target], c->secondChild->lower[target]))
            {
                upper_clusters.push_back(cluster);
            }

            if (FLOAT_EQUAL(cluster->upper[target], c->firstChild->upper[target]))
            {
                lower_clusters.push_back(cluster);
            }
        }

        if (delta > 0)
        {
            float new_pos = c->firstChild->upper[target] + delta;
            for (Cluster *cluster : upper_clusters)
                if (new_pos > cluster->upper[target] - thiccness)
                    return;
        }
        else
        {
            float new_pos = c->secondChild->lower[target] + delta;
            for (Cluster *cluster : lower_clusters)
                if (new_pos < cluster->lower[target] + thiccness)
                    return;
        }

        for (Cluster *cluster : upper_clusters)
            cluster->lower[target] += delta;

        for (Cluster *cluster : lower_clusters)
            cluster->upper[target] += delta;
    }

    ClusterManager() = default;

    ~ClusterManager()
    {

    }
};

#endif // CLUSTER_HH