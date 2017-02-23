/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

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

#include "../account_repository.hpp"
#include <crypto/hash.hpp>
#include <repository/world_state_repository.hpp>
#include <transaction_builder/transaction_builder.hpp>
#include <util/logger.hpp>
#include <util/convert_string.hpp>

constexpr auto NameSpaceID = "account repository";

namespace repository {
namespace account {

namespace detail {
/********************************************************************************************
 * stringify / parse
 ********************************************************************************************/
std::string stringifyAccount(const Api::Account &obj) {
  std::string ret;
  obj.SerializeToString(&ret);
  return ret;
}

Api::Account parseAccount(const std::string &str) {
  Api::Account ret;
  ret.ParseFromString(str);
  return ret;
}

/********************************************************************************************
 * utils
 ********************************************************************************************/
std::string createAccountUuid(const std::string &publicKey) {
  /*
      account-uuid (Reason for account-uuid is url param can not use base64 in
     default,
      And accout-uuid is sha3-256(publicKey) )
  */
  return hash::sha3_256_hex(publicKey);
}
}

/********************************************************************************************
 * Add<Account>
 ********************************************************************************************/
std::string add(const std::string &publicKey, const std::string &name,
                const std::vector<std::string> &assets) {

  const auto allAssets = convert_string::to_string(assets);

  logger::explore(NameSpaceID) << "Add<Account> publicKey: " << publicKey
                               << " name: " << name << " assets: " << allAssets;

  const auto uuid = detail::createAccountUuid(publicKey);
  const auto rval = world_state_repository::find(uuid);

  if (rval.empty()) {
    const auto account = txbuilder::createAccount(publicKey, name, assets);
    const auto strAccount = detail::stringifyAccount(account);
    logger::debug(NameSpaceID) << "Add key: " << uuid << " strAccount: \""
                               << strAccount << "\"";
    if (world_state_repository::add(uuid, strAccount)) {
      return uuid;
    }
  }

  return "";
}

/********************************************************************************************
 * Add<Asset, To<Account>>
 ********************************************************************************************/
bool attach(const std::string &uuid, const std::string &asset) {

  const auto strAccount = world_state_repository::find(uuid);

  if (strAccount.empty()) {
    return false;
  }

  Api::Account account = detail::parseAccount(strAccount);
  account.add_assets(asset);

  if (world_state_repository::update(uuid, detail::stringifyAccount(account))) {
    logger::explore(NameSpaceID) << "Add<Asset, To<Account>> uuid: " << uuid
                                 << "asset: " << asset;
    return true;
  }

  return false;
}

/********************************************************************************************
 * Update<Account>
 ********************************************************************************************/
bool update(const std::string &uuid, const std::vector<std::string> &assets) {

  // for logger::explore
  // This should be on util. It's better to be able to write each element string
  // format.
  const auto allAssets = [&] {
    std::string ret;
    for (const auto &e : assets)
      ret += e + ", ";
    return ret;
  }();

  logger::explore(NameSpaceID) << "Update<Account> uuid: " << uuid
                               << " assets: " << allAssets;
  const auto rval = world_state_repository::find(uuid);
  if (not rval.empty()) {
    const auto account = detail::parseAccount(rval);
    const auto strAccount = detail::stringifyAccount(account);
    if (world_state_repository::update(uuid, strAccount)) {
      logger::debug(NameSpaceID) << "Update strAccount: \"" << strAccount
                                 << "\"";
      return true;
    }
  }

  return false;
}
/********************************************************************************************
 * Remove<Account>
 ********************************************************************************************/
bool remove(const std::string &uuid) {
  logger::explore(NameSpaceID) << "Remove<Account> uuid: " << uuid;
  return world_state_repository::remove(uuid);
}

/********************************************************************************************
 * find
 ********************************************************************************************/
Api::Account findByUuid(const std::string &uuid) {
  const auto strAccount = world_state_repository::find(uuid);
  logger::debug(NameSpaceID) << "findByUuid(): " << strAccount;
  if (not strAccount.empty()) {
    return detail::parseAccount(strAccount);
  }
  return Api::Account();
}
}
}

/*
    // This is for SimpleAsset
        // SampleAsset has only quantity no logic, so this value is int.
        bool update_quantity(const std::string& uuid, const std::string&
   assetName,
            std::int64_t newValue) {

            const auto strAccount  = world_state_repository::find(uuid);

            Api::Account account = detail::parseAccount(serializedAccount);

            for (auto& asset: account.assets) {
                if (asset.name == assetName) {  // asset.name == assetName (can
   adapt struct?)
                    asset = newValue;      // asset.value = newValue
                }
            }

            return world_state_repository::update(uuid,
   detail::stringifyAccount(account));
        }
*/