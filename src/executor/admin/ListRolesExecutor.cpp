/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "executor/admin/ListRolesExecutor.h"
#include "context/QueryContext.h"
#include "planner/Admin.h"
#include "service/PermissionManager.h"

namespace nebula {
namespace graph {

folly::Future<Status> ListRolesExecutor::execute() {
    SCOPED_TIMER(&execTime_);
    return listRoles();
}

folly::Future<Status> ListRolesExecutor::listRoles() {
    auto *lrNode = asNode<ListRoles>(node());
    return qctx()
        ->getMetaClient()
        ->listRoles(lrNode->space())
        .via(runner())
        .then([this](StatusOr<std::vector<meta::cpp2::RoleItem>> &&resp) {
            SCOPED_TIMER(&execTime_);
            if (!resp.ok()) {
                return std::move(resp).status();
            }
            nebula::DataSet v({"Account", "Role Type"});
            auto items = std::move(resp).value();
            // Only god and admin show all roles, other roles only show themselves
            const auto &account = qctx_->rctx()->session()->user();
            auto foundItem = std::find_if(items.begin(), items.end(), [&account](const auto &item) {
                return item.get_user_id() == account;
            });
            if (foundItem != items.end() &&
                foundItem->get_role_type() != meta::cpp2::RoleType::ADMIN) {
                v.emplace_back(
                    Row({foundItem->get_user_id(),
                         meta::cpp2::_RoleType_VALUES_TO_NAMES.at(foundItem->get_role_type())}));
            } else {
                for (const auto &item : items) {
                    v.emplace_back(nebula::Row(
                        {item.get_user_id(),
                         meta::cpp2::_RoleType_VALUES_TO_NAMES.at(item.get_role_type())}));
                }
            }
            return finish(std::move(v));
        });
}

}   // namespace graph
}   // namespace nebula
