/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "common/base/Base.h"

#include "validator/test/ValidatorTestBase.h"

DECLARE_uint32(max_allowed_statements);

namespace nebula {
namespace graph {

class FindPathValidatorTest : public ValidatorTestBase {
public:
};

using PK = nebula::graph::PlanNode::Kind;

TEST_F(FindPathValidatorTest, SinglePairPath) {
    {
        std::string query = "FIND SHORTEST PATH FROM \"1\" TO \"2\" OVER like UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kStart,
            PK::kConjunctPath,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kBFSShortest,
            PK::kBFSShortest,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query = "FIND SHORTEST PATH FROM \"1\" TO \"2\" OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kStart,
            PK::kConjunctPath,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kBFSShortest,
            PK::kBFSShortest,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query = "FIND SHORTEST PATH FROM \"1\" TO \"2\" OVER * UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kStart,
            PK::kConjunctPath,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kBFSShortest,
            PK::kBFSShortest,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
}

TEST_F(FindPathValidatorTest, MultiPairPath) {
    {
        std::string query = "FIND SHORTEST PATH FROM \"1\" TO \"2\",\"3\" OVER like UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kStart,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query = "FIND SHORTEST PATH FROM \"1\" TO \"2\",\"3\" OVER *";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kStart,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query =
            "FIND SHORTEST PATH FROM \"1\",\"2\" TO \"3\",\"4\" OVER like UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kStart,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
}

TEST_F(FindPathValidatorTest, ALLPath) {
    {
        std::string query = "FIND ALL PATH FROM \"1\" TO \"2\" OVER like UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kProject,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kStart,
            PK::kProject,
            PK::kProject,
            PK::kProduceAllPaths,
            PK::kProduceAllPaths,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query = "FIND ALL PATH FROM \"1\" TO \"2\",\"3\" OVER like UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kProject,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kStart,
            PK::kProject,
            PK::kProject,
            PK::kProduceAllPaths,
            PK::kProduceAllPaths,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query = "FIND ALL PATH FROM \"1\",\"2\" TO \"3\",\"4\" OVER like UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kProject,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kStart,
            PK::kProject,
            PK::kProject,
            PK::kProduceAllPaths,
            PK::kProduceAllPaths,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query = "FIND ALL PATH FROM \"1\" TO \"2\" OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kProject,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kStart,
            PK::kProject,
            PK::kProject,
            PK::kProduceAllPaths,
            PK::kProduceAllPaths,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kPassThrough,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
}

TEST_F(FindPathValidatorTest, RunTimePath) {
    {
        std::string query =
            "GO FROM \"1\" OVER like YIELD like._src AS src, like._dst AS dst "
            " | FIND SHORTEST PATH FROM $-.src TO $-.dst OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kDedup,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kDedup,
            PK::kPassThrough,
            PK::kProject,
            PK::kStart,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query =
            "GO FROM \"1\" OVER like YIELD like._src AS src, like._dst AS dst "
            " | FIND ALL PATH FROM $-.src TO $-.dst OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kProject,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kProduceAllPaths,
            PK::kProduceAllPaths,
            PK::kDedup,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kProject,
            PK::kPassThrough,
            PK::kProject,
            PK::kStart,
            PK::kGetNeighbors,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query =
            "GO FROM \"1\" OVER like YIELD like._src AS src, like._dst AS dst "
            " | FIND SHORTEST PATH FROM \"2\" TO $-.dst OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kDedup,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kProject,
            PK::kPassThrough,
            PK::kGetNeighbors,
            PK::kStart,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query =
            "GO FROM \"1\" OVER like YIELD like._src AS src, like._dst AS dst "
            " | FIND SHORTEST PATH FROM $-.src TO \"2\" OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kDedup,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kProject,
            PK::kPassThrough,
            PK::kGetNeighbors,
            PK::kStart,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query =
            "$a = GO FROM \"1\" OVER like YIELD like._src AS src, like._dst AS dst; "
            "FIND SHORTEST PATH FROM $a.src TO $a.dst OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kDedup,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kDedup,
            PK::kPassThrough,
            PK::kProject,
            PK::kStart,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query = "$a = GO FROM \"1\" OVER like yield like._src AS src; "
                            "GO FROM \"2\" OVER like yield like._src AS src, like._dst AS dst "
                            " | FIND SHORTEST PATH FROM $a.src TO $-.dst OVER like UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kDedup,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kDedup,
            PK::kPassThrough,
            PK::kProject,
            PK::kStart,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query =
            "YIELD \"1\" AS src, \"2\" AS dst"
            " | FIND SHORTEST PATH FROM $-.src TO $-.dst OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kCartesianProduct,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kDedup,
            PK::kProduceSemiShortestPath,
            PK::kProduceSemiShortestPath,
            PK::kProject,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kDedup,
            PK::kPassThrough,
            PK::kProject,
            PK::kStart,
            PK::kProject,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
    {
        std::string query = "YIELD \"1\" AS src, \"2\" AS dst"
                            " | FIND ALL PATH FROM $-.src TO $-.dst OVER like, serve UPTO 5 STEPS";
        std::vector<PlanNode::Kind> expected = {
            PK::kDataCollect,
            PK::kLoop,
            PK::kProject,
            PK::kConjunctPath,
            PK::kProject,
            PK::kDedup,
            PK::kDedup,
            PK::kDedup,
            PK::kProject,
            PK::kProject,
            PK::kProject,
            PK::kProduceAllPaths,
            PK::kProduceAllPaths,
            PK::kDedup,
            PK::kGetNeighbors,
            PK::kGetNeighbors,
            PK::kProject,
            PK::kPassThrough,
            PK::kProject,
            PK::kStart,
            PK::kStart,
        };
        EXPECT_TRUE(checkResult(query, expected));
    }
}

}   // namespace graph
}   // namespace nebula

