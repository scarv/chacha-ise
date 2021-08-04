# Copyright (C) 2021 SCARV project <info@scarv.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found at https://opensource.org/licenses/MIT (or should be included 
# as LICENSE.txt within the associated archive or repository).

ifndef CCI_HOME
  $(error "execute 'source ./bin/conf.sh' to configure environment")
endif

# =============================================================================
COMPONENTS = gcc pk spike

all	   : clone build

define rule_clone
$(strip ${1})-clone  :
	@$${CCI_HOME}/toolchain-rvb/$(strip ${1})-clone.sh
endef

define rule_build
$(strip ${1})-build  :
	@$${CCI_HOME}/toolchain-rvb/$(strip ${1})-build.sh
endef

$(foreach COMPONENT,${COMPONENTS},$(eval $(call rule_clone, ${COMPONENT})))
$(foreach COMPONENT,${COMPONENTS},$(eval $(call rule_build, ${COMPONENT})))

clone  : $(foreach COMPONENT, ${COMPONENTS}, ${COMPONENT}-clone )

build  : $(foreach COMPONENT, ${COMPONENTS}, ${COMPONENT}-build )

clean  :
	@rm --force --recursive ${CCI_HOME}/build/toolchain-rvb

# =============================================================================
