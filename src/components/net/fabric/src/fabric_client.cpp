/*
   Copyright [2017-2019] [IBM Corporation]
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "fabric_client.h"

Fabric_client::Fabric_client(Fabric &fabric_, event_producer &ep_, ::fi_info & info_, const std::string & remote_, std::uint16_t control_port_)
  : Fabric_connection_client(fabric_, ep_, info_, remote_, control_port_)
{}

Fabric_client::~Fabric_client()
{}
