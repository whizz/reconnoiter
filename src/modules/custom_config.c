/*
 * Copyright (c) 2014, Circonus, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name Circonus, Inc. nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "noit_defines.h"
#include "noit_module.h"
#include "noit_check.h"
#include "noit_check_tools.h"
#include "utils/noit_log.h"
#include "utils/noit_hash.h"
#include "utils/noit_btrie.h"
#include "custom_config.xmlh"
#include <assert.h>

static int custom_config_module_id = -1;

static int
custom_config_onload(noit_image_t *self) {
  int i, cnt;
  noit_conf_section_t *acl_c;
  custom_config_module_id = noit_check_register_module("custom");
  if(custom_config_module_id < 0) return -1;
  return 0;
}

static int
custom_config_config(noit_module_generic_t *self, noit_hash_table *o) {
  return 0;
}

static noit_hook_return_t
custom_config_hook_impl(void *closure, noit_check_t *check) {
  noit_hash_table *config;
  config = noit_check_get_module_config(check, custom_config_module_id);
  if(config && noit_hash_size(config)) {
    noit_hash_merge_as_dict(check->config, config);
  }
  return NOIT_HOOK_CONTINUE;
}

static int
custom_config_init(noit_module_generic_t *self) {
  check_config_fixup_hook_register("custom_config", custom_config_hook_impl, NULL);
  return 0;
}

noit_module_generic_t custom_config = {
  {
    .magic = NOIT_GENERIC_MAGIC,
    .version = NOIT_GENERIC_ABI_VERSION,
    .name = "custom_config",
    .description = "config namespaces",
    .xml_description = custom_config_xml_description,
    .onload = custom_config_onload
  },
  custom_config_config,
  custom_config_init
};

