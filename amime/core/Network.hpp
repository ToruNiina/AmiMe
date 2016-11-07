#ifndef AMIME_CORE_NETWORK
#define AMIME_CORE_NETWORK
#include <unordered_map>
#include <amime/core/Node.hpp>

namespace amime
{

template<typename traitsT>
class Network
{
  public:
    typedef traitsT traits_type;
    typedef typename traits_type::time_type time_type;
    typedef typename traits_type::state_type state_type;
    typedef typename traits_type::node_id_type node_id_type;
    typedef Node<state_type, time_type> node_type;
    typedef std::unordered_map<node_id_type, node_type> container_type;
    typedef typename container_type::iterator       iterator;
    typedef typename container_type::const_iterator const_iterator;

  public:
    Network() = default;
    ~Network() = default;

    template<typename ... argT>
    std::pair<iterator, bool>
    emplace(argT&& ...args)
    {
        return this->container_.emplace(std::forward<argT...>(args...));
    }

    std::pair<iterator, bool>
    insert(const node_id_type& id, node_type&& node)
    {
        return this->container_.insert(std::make_pair(id, std::forward<node_type>(node)));
    }
    std::pair<iterator, bool>
    insert(const node_id_type& id, const node_type& node)
    {
        return this->container_.insert(std::make_pair(id, node));
    }

    void connect(const node_id_type& fw, const node_id_type& bw)
    {
        this->container_.at(bw).inputs.push_back(&(this->container_.at(fw)));
    }

    void erase(const node_id_type& id){container_.erase(id);}
    void erase(const_iterator iter)   {container_.erase(iter);}
    void clear(){container_.clear();}
    std::size_t size(){return container_.size();}

    std::size_t   count(const node_id_type& id) const {return container_.count(id);}
    iterator       find(const node_id_type& id)       {return container_.find(id);}
    const_iterator find(const node_id_type& id) const {return container_.find(id);}

    node_type&       at(const node_id_type& id)       {return container_.at(id);}
    node_type const& at(const node_id_type& id) const {return container_.at(id);}
    node_type&       operator[](const node_id_type& id)       {return container_[id];}
    node_type const& operator[](const node_id_type& id) const {return container_[id];}

    iterator begin() {return container_.begin();}
    iterator end()   {return container_.end();}
    const_iterator cbegin() const {return container_.cbegin();}
    const_iterator cend()   const {return container_.cend();}

  private:

    container_type container_;
};

} // amime
#endif /* AMIME_CORE_NETWORK */
