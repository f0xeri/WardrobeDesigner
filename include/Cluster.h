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


class Claster
{
private:
    glm::vec2 lower;
    glm::vec2 upper;
    Claster *firstChild;
    Claster *secondChild;
    Claster *parent;

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

    Claster(glm::vec2 lower, glm::vec2 upper, Claster *parent = nullptr)
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
            firstChild = new Claster(lower, glm::vec2(lower.x + separator_offset, upper.y), this);
            secondChild = new Claster(lower + glm::vec2(separator_offset + separator_width, 0), upper, this);
        }
        else
        {
            firstChild = new Claster(lower, glm::vec2(upper.x, lower.y + separator_offset), this);
            secondChild = new Claster(lower + glm::vec2(0, separator_offset + separator_width), upper, this);
        }
    }

    void split(glm::vec2 separator_pos, float separator_width, bool isVertical)
    {
        if (isVertical)
        {
            firstChild = new Claster(lower, glm::vec2(separator_pos.x , upper.y), this);
            secondChild = new Claster(glm::vec2(separator_pos.x + separator_width, lower.y), upper, this);
        }
        else
        {
            firstChild = new Claster(lower, glm::vec2(upper.x, separator_pos.y ), this);
            secondChild = new Claster(glm::vec2(lower.x, separator_pos.y + separator_width), upper, this);
        }
    }

    Claster *to_local_claster(glm::vec2 pos)
    {
        if (is_leaf())
        {
            return this;
        }

        int target = !is_vertical();

        if (pos[target] < firstChild->upper[target])
        {
            return firstChild->to_local_claster(pos);
        }

        if (pos[target] > secondChild->lower[target])
        {
            return secondChild->to_local_claster(pos);
        }

        return nullptr;
    }

    ~Claster(){

    };

    friend class Claster_manager;
};

class Claster_manager
{
private:
    Claster *root;
    std::vector<Claster *> clasters;
    glm::vec3 origin;
    float depth;
    
public:
    glm::vec3 get_pos(Claster *c)
    {
        if(c->is_vertical())
            return glm::vec3(c->firstChild->upper.x, c->firstChild->lower.y,-depth);
        return glm::vec3(c->firstChild->lower.x, c->firstChild->upper.y,-depth);    
    }

    glm::vec3 get_scale(Claster *c)
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

    Claster_manager(glm::vec3 size, glm::vec3 origin) : origin(origin)
    {
        glm::vec2 size2d = glm::vec2(size.x, size.y);
        glm::vec2 origin2d = glm::vec2(origin.x, origin.y);
        depth = size.z;
        root = new Claster(origin2d, origin2d + size2d);
        clasters.push_back(root);
    }

    Claster *try_split(glm::vec3 pos, float width, bool isVertical)
    {
        Claster *c = root->to_local_claster(glm::vec2(pos.x, pos.y));
        if (c)
        {
            c->split(glm::vec2(pos.x, pos.y), width, isVertical);
            clasters.push_back(c->firstChild);
            clasters.push_back(c->secondChild);
        }
        return c;
    }

    void try_move_separator(Claster *c, float delta)
    {
        int target = !c->is_vertical();
        std::vector<Claster *> upper_clusters;
        std::vector<Claster *> lower_clusters;
        
        float thiccness = c->secondChild->lower[target] - c->firstChild->upper[target] ;
        for (Claster *claster : clasters)
        {
            if (FLOAT_EQUAL(claster->lower[target], c->secondChild->lower[target]))
            {
                upper_clusters.push_back(claster);
            }

            if (FLOAT_EQUAL(claster->upper[target], c->firstChild->upper[target]))
            {
                lower_clusters.push_back(claster);
            }
        }

        if (delta > 0)
        {
            float new_pos = c->firstChild->upper[target] + delta;
            for (Claster *claster : upper_clusters)
                if (new_pos > claster->upper[target] - thiccness)
                    return;
        }
        else
        {
            float new_pos = c->secondChild->lower[target] + delta;
            for (Claster *claster : lower_clusters)
                if (new_pos < claster->lower[target] + thiccness)
                    return;
        }

        for (Claster *claster : upper_clusters)
            claster->lower[target] += delta;

        for (Claster *claster : lower_clusters)
            claster->upper[target] += delta;
    }

    Claster_manager() = default;

    ~Claster_manager()
    {
    }
};

#endif // CLUSTER_HH
