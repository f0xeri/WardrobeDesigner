#include "Cluster.h"

using namespace std;

void Cluster::constraint_by_children(float &max_up_border, float &max_down_border)
{
    stack<pair<Cluster *, float>> s;

    float relative_up_border = 0;
    float relative_down_border = claster_scale[!isVertical];

    s.push({firstChild, 0});
    while (!s.empty())
    {
        pair<Cluster *, float> p = s.top();
        s.pop();
        if (!p.first->is_leaf())
        {
            s.push({p.first->firstChild, p.second});
            if (p.first->isVertical == isVertical)
            {
                s.push({p.first->secondChild, p.second + p.first->separator_offset + p.first->separator_width});
            }
            else
            {
                s.push({p.first->secondChild, p.second});
            }
        }
        else
        {
            if (p.second > relative_up_border)
                relative_up_border = p.second;
        }
    }

    s.push({secondChild, claster_scale[!isVertical]});
    while (!s.empty())
    {
        pair<Cluster *, float> p = s.top();
        s.pop();
        if (!p.first->is_leaf())
        {
            s.push({p.first->secondChild, p.second});
            if (p.first->isVertical == isVertical)
            {
                s.push({p.first->firstChild, p.second - (p.first->claster_scale[!isVertical] - p.first->separator_offset /* + p.first->separator_width*/)});
            }
            else
            {
                s.push({p.first->firstChild, p.second});
            }
        }
        else
        {
            if (p.second < relative_down_border)
                relative_down_border = p.second;
        }
    }

    max_down_border = max_up_border + relative_down_border;
    max_up_border += relative_up_border;
}

Cluster::Cluster(float width, float height, glm::vec2 origin, Cluster *parent)
{
    firstChild = nullptr;
    secondChild = nullptr;
    this->parent = parent;
    this->origin = origin;
    isVertical = false;
    separator_offset = 0;
    separator_width = 0;
    claster_scale = glm::vec2(width, height);
}

void Cluster::split(float separator_offset, float separator_width, bool isVertical)
{
    this->separator_offset = separator_offset;
    this->separator_width = separator_width;
    this->isVertical = isVertical;
    if (isVertical)
    {
        firstChild = new Cluster(separator_offset, claster_scale.y,origin, this);
        secondChild = new Cluster(claster_scale.x - separator_offset - separator_width, claster_scale.y,origin + glm::vec2(separator_offset - separator_width,0), this);
    }
    else
    {
        firstChild = new Cluster(claster_scale.x, separator_offset,origin, this);
        secondChild = new Cluster(claster_scale.x, claster_scale.y - separator_offset - separator_width,origin + glm::vec2(0,separator_offset - separator_width), this);
    }
    Cluster* c = this;
    while (!c->is_root())
    {
        c->find_constraints();
        c = c->parent;
    }
    c->find_constraints();
}

void Cluster::find_constraints()
{
    Cluster *c = this;
    std::vector<std::pair<Cluster *, bool>> v;
    c = c->parent;
    while (c)
    {
        if (c->isVertical == isVertical)
            v.push_back({c, c == c->secondChild});
        c = c->parent;
    }
    float beg = 0;
    for (auto p : v)
        if (p.second)
            beg += p.first->separator_offset + p.first->separator_width;
    float end = 0;
    constraint_by_children(beg, end);
    BMin = beg;
    BMax = end;
}

std::tuple<Cluster *, glm::vec2, glm::vec2> Cluster::to_local_claster(glm::vec2 pos,glm::vec2 origin)
{
    if (is_leaf())
    {
        return {this, origin,pos};
    }

    int target = !isVertical;

    if (pos[target] < separator_offset)
    {
        return firstChild->to_local_claster(pos,origin);
    }

    if (pos[target] > separator_offset + separator_width)
    {
        pos[target] = pos[target] - (separator_offset + separator_width);
        origin[target] += (separator_offset + separator_width);
        return secondChild->to_local_claster(pos,origin);
    }

    return {nullptr, glm::vec2(),glm::vec2()};
}

void Cluster::move_separator(float delta)
{
    std::stack<Cluster *> s;
    std::vector<Cluster *> v;
    s.push(firstChild);

    while (!s.empty())
    {
        Cluster *c = s.top(); s.pop();
        v.push_back(c);
        if (!c->is_leaf())
        {
            if (c->isVertical != isVertical)
            {
                s.push(c->firstChild);
                s.push(c->secondChild);
            }
            else
            {
                s.push(c->secondChild);
            }
        }
    }

    for (auto c : v)
    {
        c->claster_scale[!isVertical] += delta;
        c->BMax += delta;
    }
    v.clear();

    s.push(secondChild);
    while (!s.empty())
    {
        Cluster *c = s.top();
        s.pop();
        v.push_back(c);
        if (!c->is_leaf())
        {
            if (c->isVertical != isVertical)
            {
                s.push(c->firstChild);
                s.push(c->secondChild);
            }
            else
            {
                s.push(c->firstChild);
            }
        }
    }

    for (auto c : v)
    {
        c->claster_scale[!isVertical] -= delta;
        c->separator_offset -= delta;
        origin[isVertical] += delta;
        c->BMin += delta;
    }

    separator_offset += delta;

    Cluster* c = this;
    while (!c->is_root())
    {
        c->find_constraints();
        c = c->parent;
    }
    c->find_constraints();
    
    
}