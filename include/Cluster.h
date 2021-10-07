#if !defined(CLUSTER_HH)
#define CLUSTER_HH

#include <memory>
#include <stack>
#include <vector>
#include <tuple>
#include <glm/vec2.hpp>

class Cluster
{
private:
    
    
    
    bool is_root()
    {
        return parent == nullptr;
    }

    bool is_leaf()
    {
        return firstChild == nullptr;
    }

    void constraint_by_children(float &max_up_border, float &max_down_border);
    
    Cluster* parent;
    void find_constraints();
public:
    bool isVertical;
    float separator_offset;
    Cluster* firstChild;
    Cluster* secondChild;
    glm::vec2 claster_scale;
    float separator_width;

    float BMax,BMin;
    Cluster(float width,float height,Cluster* parent = nullptr);
    Cluster() = default;
    void split(float separator_offset,float separator_width,bool isVertical);
    std::tuple<Cluster *, glm::vec2, glm::vec2>  to_local_claster(glm::vec2 pos,glm::vec2 origin = glm::vec2());
    void move_separator(float delta);

    ~Cluster()
    {
        
    };
};


#endif // CLUSTER_HH
