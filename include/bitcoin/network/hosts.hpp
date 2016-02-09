/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_NETWORK_HOSTS_HPP
#define LIBBITCOIN_NETWORK_HOSTS_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
#include <boost/circular_buffer.hpp>
#include <boost/filesystem.hpp>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/network/define.hpp>
#include <bitcoin/network/settings.hpp>

namespace libbitcoin {
namespace network {

/// The hosts class manages a thread-safe dynamic store of network addresses.
/// The store can be loaded and saved from/to the specified file path.
/// The file is a line-oriented set of config::authority serializations.
/// Duplicate addresses and those with zero-valued ports are disacarded.
class BCT_API hosts
{
public:
    typedef boost::filesystem::path path;
    typedef message::network_address address;
    typedef std::function<void(bool)> truth_handler;
    typedef std::function<void(size_t)> count_handler;
    typedef std::function<void(const code&)> result_handler;
    typedef std::function<void(const code&, const address&)> fetch_handler;

    /// Construct an instance.
    hosts(threadpool& pool, const settings& settings);

    /// This class is not copyable.
    hosts(const hosts&) = delete;
    void operator=(const hosts&) = delete;

    virtual void count(count_handler handler);
    virtual void store(const address& host, result_handler handler);
    virtual void store(const address::list& hosts, result_handler handler);
    virtual void remove(const address& host, result_handler handler);
    virtual void load(result_handler handler);
    virtual void save(result_handler handler);
    virtual void fetch(fetch_handler handler);

private:
    typedef boost::circular_buffer<address> list;
    typedef list::iterator iterator;

    iterator find(const address& host);

    void do_count(count_handler handler);
    void do_store(const address& host, result_handler handler);
    void do_remove(const address& host, result_handler handler);
    void do_load(const path& file_path, result_handler handler);
    void do_save(const path& file_path, result_handler handler);
    void do_fetch(fetch_handler handler);
    size_t select_random_host();

    list buffer_;
    dispatcher dispatch_;
    boost::filesystem::path file_path_;

    // HACK: we use this because the buffer capacity cannot be set to zero.
    const bool disabled_;
};

} // namespace network
} // namespace libbitcoin

#endif

