#ifndef __KITE_PLUGIN_COMPONENT_H_
#define __KITE_PLUGIN_COMPONENT_H_

#include <api/ado_itf.h>
#include <limits>

#define VALUE_SIZE 256

const uint64_t MAGIC_NUMBER = std::numeric_limits<uint64_t>::max();

class ADO_kite_plugin : public Component::IADO_plugin {
private:
  static constexpr bool option_DEBUG = true;

public:
  /**
   * Constructor
   *
   * @param block_device Block device interface
   *
   */
  ADO_kite_plugin() {}

  /**
   * Destructor
   *
   */
  virtual ~ADO_kite_plugin() {}
  /**
   * Component/interface management
   *
   */
  DECLARE_VERSION(0.1f);
  DECLARE_COMPONENT_UUID(0x59564581, 0x9e1b, 0x4811, 0xbdb2, 0x19, 0x57, 0xa0,
                         0xa6, 0x84, 0x58);

  void *query_interface(Component::uuid_t &itf_uuid) override {
    if (itf_uuid == Component::IADO_plugin::iid()) {
      return (void *)static_cast<Component::IADO_plugin *>(this);
    } else
      return NULL; // we don't support this interface
  }

  void unload() override { delete this; }

public:
  /* IADO_plugin */
  status_t register_mapped_memory(void *shard_vaddr, void *local_vaddr,
                                  size_t len) override;

  status_t do_work(
      const uint64_t work_key, const std::string &key, void *shard_value_vaddr,
      size_t value_len,
      const void *in_work_request, /* don't use iovec because of non-const */
      const size_t in_work_request_len, void *&out_work_response,
      size_t &out_work_response_len) override;

  status_t shutdown() override;
};

#endif
