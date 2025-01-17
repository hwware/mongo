/**
 *    Copyright (C) 2019-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include <string>
#include <vector>

#include "mongo/base/status.h"
#include "mongo/base/string_data.h"
#include "mongo/client/sasl_client_conversation.h"
#include "mongo/client/sasl_client_session.h"
#include "mongo/client/sasl_iam_protocol_common.h"

namespace mongo {
/**
 *  Client side authentication session for MONGODB-IAM SASL.
 */
class SaslIAMClientConversation : public SaslClientConversation {
    SaslIAMClientConversation(const SaslIAMClientConversation&) = delete;
    SaslIAMClientConversation& operator=(const SaslIAMClientConversation&) = delete;

public:
    explicit SaslIAMClientConversation(SaslClientSession* saslClientSession);

    virtual ~SaslIAMClientConversation() = default;

    StatusWith<bool> step(StringData inputData, std::string* outputData) override;

private:
    /**
     * Get AWS credentials either from the SASL session or a local HTTP server.
     */
    iam::AWSCredentials _getCredentials() const;

    /**
     * Get AWS credentials from SASL session.
     */
    iam::AWSCredentials _getUserCredentials() const;

    StatusWith<bool> _firstStep(std::string* outputData);
    StatusWith<bool> _secondStep(StringData inputData, std::string* outputData);

private:
    // Step of protocol - either 1 or 2
    std::uint32_t _step{0};

    // Client nonce
    std::vector<char> _clientNonce;
};

}  // namespace mongo
