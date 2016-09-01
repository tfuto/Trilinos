#ifndef BULK_DATA_ID_MAPPER_HPP
#define BULK_DATA_ID_MAPPER_HPP

#include <stk_mesh/base/Types.hpp>
#include <stk_mesh/base/BulkData.hpp>
#include "stk_mesh/base/GetEntities.hpp"
#include "ElemElemGraphImpl.hpp"
#include "ElemGraphCoincidentElems.hpp"

namespace stk
{
namespace mesh
{
namespace impl
{

template<typename LocalIDType>
class LocalIdMapperT
{
public:
    LocalIdMapperT()
    : entityToLocalId()
    {
    }

    LocalIdMapperT(const stk::mesh::BulkData &bulk, stk::mesh::EntityRank rank)
    : entityToLocalId()
    {
        set_local_ids(bulk, rank);
    }

    const LocalIDType INVALID_LOCAL_ID = std::numeric_limits<LocalIDType>::max();

    void set_size(const stk::mesh::BulkData &bulk)
    {
        entityToLocalId.resize(bulk.get_size_of_entity_index_space(), INVALID_LOCAL_ID);
    }

    void clear()
    {
        entityToLocalId.clear();
    }

    void set_local_ids(const stk::mesh::BulkData &bulk, stk::mesh::EntityRank rank)
    {
        entityToLocalId.resize(bulk.get_size_of_entity_index_space(), INVALID_LOCAL_ID);
        if(bulk.mesh_meta_data().entity_rank_count() >= rank)
        {
            const stk::mesh::BucketVector & elemBuckets = bulk.get_buckets(rank, bulk.mesh_meta_data().locally_owned_part());
            LocalIDType localId = 0;
            for(size_t i=0; i<elemBuckets.size(); ++i)
            {
                const stk::mesh::Bucket& bucket = *elemBuckets[i];
                for(size_t j=0; j<bucket.size(); ++j)
                {
                    add_new_entity_with_local_id(bucket[j], localId);
                    localId++;
                }
            }
        }
    }

    bool does_entity_have_local_id(stk::mesh::Entity entity) const
    {
        return (entity.local_offset() < entityToLocalId.size() && entityToLocalId[entity.local_offset()] != INVALID_LOCAL_ID);
    }

    LocalIDType entity_to_local(stk::mesh::Entity entity) const
    {
        ThrowAssert(entityToLocalId.size() > entity.local_offset());

        return entityToLocalId[entity.local_offset()];
    }

    void make_space_for_new_elements(const stk::mesh::EntityVector & elements)
    {
        stk::mesh::Entity maxEntity;
        maxEntity.set_local_offset(0);
        for(stk::mesh::Entity elem : elements)
            maxEntity.set_local_offset(std::max(maxEntity.local_offset(), elem.local_offset()));

        if(maxEntity.local_offset() >= entityToLocalId.size())
            entityToLocalId.resize(maxEntity.local_offset() + 1, INVALID_LOCAL_ID);
    }

    void create_local_ids_for_elements(const stk::mesh::EntityVector & elements)
    {
        make_space_for_new_elements(elements);
        for(size_t localId = 0; localId < elements.size(); localId++)
            add_new_entity_with_local_id(elements[localId], localId);
    }

    void add_new_entity_with_local_id(stk::mesh::Entity elem, LocalIDType id)
    {
        entityToLocalId[elem.local_offset()] = id;
    }

    void clear_local_id_for_elem(stk::mesh::Entity elem)
    {
        entityToLocalId[elem.local_offset()] = INVALID_LOCAL_ID;
    }

private:
    std::vector<LocalIDType> entityToLocalId;
};

typedef LocalIdMapperT<unsigned> LocalIdMapper;

class ElementLocalIdMapper
{
public:
    ElementLocalIdMapper()
    : localIdToElement(), elementToLocalIdMapper()
    {
    }

    void initialize(const stk::mesh::BulkData &bulk)
    {
        unsigned numElems = impl::get_num_local_elems(bulk);
        localIdToElement.resize(numElems, Entity());
        elementToLocalIdMapper.set_size(bulk);

        if(bulk.mesh_meta_data().entity_rank_count() >= stk::topology::ELEM_RANK)
        {
            const stk::mesh::BucketVector & elemBuckets = bulk.get_buckets(stk::topology::ELEM_RANK, bulk.mesh_meta_data().locally_owned_part());
            impl::LocalId localId = 0;
            for(size_t i=0; i<elemBuckets.size(); ++i)
            {
                const stk::mesh::Bucket& bucket = *elemBuckets[i];
                for(size_t j=0; j<bucket.size(); ++j)
                {
                    add_new_entity_with_local_id(bucket[j], localId);
                    localId++;
                }
            }
        }
    }
    void clear()
    {
        localIdToElement.clear();
        elementToLocalIdMapper.clear();
    }
    stk::mesh::Entity local_to_entity(stk::mesh::impl::LocalId local) const
    {
        return localIdToElement[local];
    }

    bool does_entity_have_local_id(stk::mesh::Entity entity) const
    {
        return elementToLocalIdMapper.does_entity_have_local_id(entity);
    }

    stk::mesh::impl::LocalId entity_to_local(stk::mesh::Entity entity) const
    {
        return elementToLocalIdMapper.entity_to_local(entity);
    }

    void make_space_for_local_id(stk::mesh::impl::LocalId localId)
    {
        if(static_cast<size_t>(localId) >= localIdToElement.size())
            localIdToElement.resize(localId + 1);
    }

    void make_space_for_new_elements(const stk::mesh::EntityVector & elements)
    {
        elementToLocalIdMapper.make_space_for_new_elements(elements);
    }

    void create_local_ids_for_elements(const stk::mesh::EntityVector & elements)
    {
        make_space_for_new_elements(elements);
        localIdToElement.resize(elements.size());
        for(size_t localId = 0; localId < elements.size(); localId++)
        {
            add_new_entity_with_local_id(elements[localId], localId);
        }
    }

    void add_new_entity_with_local_id(stk::mesh::Entity elem, impl::LocalId id)
    {
        localIdToElement[id] = elem;
        elementToLocalIdMapper.add_new_entity_with_local_id(elem, id);
    }

    void clear_local_id_for_elem(stk::mesh::Entity elem)
    {
        localIdToElement[entity_to_local(elem)] = stk::mesh::Entity::InvalidEntity;
        elementToLocalIdMapper.clear_local_id_for_elem(elem);
    }
private:
    stk::mesh::EntityVector localIdToElement;
    LocalIdMapperT<impl::LocalId> elementToLocalIdMapper;
};

class BulkDataIdMapper : public IdMapper
{
public:
    BulkDataIdMapper(const stk::mesh::BulkData &b,
                     const ElementLocalIdMapper & l)
    : bulk(b), localIdMaps(l)
    {
    }

    virtual stk::mesh::EntityId localToGlobal(stk::mesh::impl::LocalId localId) const
    {
        if(localId < 0)
            return -localId;
        else
            return bulk.identifier(localIdMaps.local_to_entity(localId));
    }
    virtual stk::mesh::impl::LocalId globalToLocal(stk::mesh::EntityId global) const
    {
        stk::mesh::Entity elem = bulk.get_entity(stk::mesh::EntityKey(stk::topology::ELEM_RANK, global));
        return localIdMaps.entity_to_local(elem);
    }

private:
    const stk::mesh::BulkData &bulk;
    const ElementLocalIdMapper & localIdMaps;
};

}
}
}

#endif
