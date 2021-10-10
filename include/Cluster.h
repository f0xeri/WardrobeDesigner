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
    glm::ivec2 lower;
    glm::ivec2 upper;

    Cluster *firstChild;
    Cluster *secondChild;
    Cluster *parent;

public:
    IObject* linked_object;
    bool isRoot()
    {
        return parent == nullptr;
    }

    bool isLeaf()
    {
        return firstChild == nullptr;
    }

    bool isVertical()
    {
        return firstChild->lower.y == secondChild->lower.y;
    }

    Cluster(glm::ivec2 lower, glm::ivec2 upper, Cluster *parent = nullptr)
    {
        firstChild = nullptr;
        secondChild = nullptr;
        this->parent = parent;
        this->lower = lower;
        this->upper = upper;
    }

    void split(glm::ivec2 separator_pos, int separator_width, bool isVertical)
    {
        if (isVertical)
        {
            firstChild = new Cluster(lower, glm::ivec2(separator_pos.x , upper.y), this);
            secondChild = new Cluster(glm::ivec2(separator_pos.x + separator_width, lower.y), upper, this);
        }
        else
        {
            firstChild = new Cluster(lower, glm::ivec2(upper.x, separator_pos.y ), this);
            secondChild = new Cluster(glm::ivec2(lower.x, separator_pos.y + separator_width), upper, this);
        }
    }

    Cluster *toLocalCluster(glm::ivec2 pos)
    {
        if (isLeaf())
        {
            return this;
        }

        int target = !isVertical();

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
    int depth;

public:
    glm::vec3 getPos(Cluster *c)
    {
        if(c->isVertical())
            return origin + glm::vec3(to_mm(c->firstChild->upper.x), to_mm(c->firstChild->lower.y),0);
        return origin + glm::vec3(to_mm(c->firstChild->lower.x), to_mm(c->firstChild->upper.y),0);
    }

    glm::vec3 getScale(Cluster *c)
    {
        if (c->isVertical())
        {
            return glm::vec3(to_mm((c->firstChild->upper.x - c->secondChild->lower.x)) * -1,
                             to_mm(c->upper.y - c->lower.y), 
                             to_mm(depth));
        }
        else
        {
            return glm::vec3(to_mm(c->upper.x - c->lower.x), 
                             to_mm(c->firstChild->upper.y - c->secondChild->lower.y) * -1, 
                             to_mm(depth));
        }
    }

    ClusterManager(glm::ivec3 size, glm::vec3 origin) : origin(origin)
    {
        glm::ivec2 size2d = glm::ivec2(size.x, size.y);
        depth = size.z;
        root = new Cluster(glm::ivec2(), size2d);
        clusters.push_back(root);
    }

    Cluster *trySplit(glm::vec3 pos, int width, bool isVertical)
    {
        glm::ivec2 pos2d = glm::ivec2(from_mm(pos.x - origin.x),from_mm(pos.y - origin.y));
        Cluster *c = root->toLocalCluster(pos2d);
        if (c)
        {
            c->split(pos2d, width, isVertical);
            clusters.push_back(c->firstChild);
            clusters.push_back(c->secondChild);
        }
        return c;
    }

    void tryMoveSeparator(Cluster *c, int delta)
    {
        int target = !c->isVertical();
        std::vector<Cluster *> upper_clusters;
        std::vector<Cluster *> lower_clusters;

        int thiccness = c->secondChild->lower[target] - c->firstChild->upper[target] ;
        for (Cluster *cluster : clusters)
        {
            if (cluster->lower[target] == c->secondChild->lower[target])
            {
                upper_clusters.push_back(cluster);
            }

            if (cluster->upper[target] == c->firstChild->upper[target])
            {
                lower_clusters.push_back(cluster);
            }
        }

        if (delta > 0)
        {
            int new_pos = c->firstChild->upper[target] + delta;
            for (Cluster *cluster : upper_clusters)
                if (new_pos > cluster->upper[target] - thiccness)
                    return;
        }
        else
        {
            int new_pos = c->secondChild->lower[target] + delta;
            for (Cluster *cluster : lower_clusters)
                if (new_pos < cluster->lower[target] + thiccness)
                    return;
        }

        for (Cluster *cluster : upper_clusters)
            cluster->lower[target] += delta;

        for (Cluster *cluster : lower_clusters)
            cluster->upper[target] += delta;
    }

    std::vector<Cluster*> getClustersToDelete(Cluster* c)
    {
        std::vector<Cluster*> toDelete = std::vector<Cluster*>();
        std::stack<Cluster*> s = std::stack<Cluster*>();
        s.push(c->firstChild);
        s.push(c->secondChild);
        while(!s.empty())
        {
            Cluster* current = s.top(); s.pop();
            toDelete.push_back(current);
            if (!current->isLeaf())
            {
                s.push(current->firstChild);
                s.push(current->secondChild);
            }
        }

        c->firstChild = nullptr;
        c->secondChild = nullptr;
        
        for (auto &cluster : toDelete)
        {
            clusters.erase(std::remove(clusters.begin(), clusters.end(), cluster), clusters.end());
        }

        return toDelete;
    }

    ClusterManager() = default;
    ~ClusterManager() = default;
};

#endif // CLUSTER_HH