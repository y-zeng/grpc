# Copyright 2017 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Autogenerated by tools/codegen/core/gen_stats_data.py

import massage_qps_stats_helpers
def massage_qps_stats(scenario_result):
  for stats in scenario_result["serverStats"] + scenario_result["clientStats"]:
    if "coreStats" not in stats: return
    core_stats = stats["coreStats"]
    del stats["coreStats"]
    stats["core_client_calls_created"] = massage_qps_stats_helpers.counter(core_stats, "client_calls_created")
    stats["core_server_calls_created"] = massage_qps_stats_helpers.counter(core_stats, "server_calls_created")
    stats["core_cqs_created"] = massage_qps_stats_helpers.counter(core_stats, "cqs_created")
    stats["core_client_channels_created"] = massage_qps_stats_helpers.counter(core_stats, "client_channels_created")
    stats["core_client_subchannels_created"] = massage_qps_stats_helpers.counter(core_stats, "client_subchannels_created")
    stats["core_server_channels_created"] = massage_qps_stats_helpers.counter(core_stats, "server_channels_created")
    stats["core_syscall_poll"] = massage_qps_stats_helpers.counter(core_stats, "syscall_poll")
    stats["core_syscall_wait"] = massage_qps_stats_helpers.counter(core_stats, "syscall_wait")
    stats["core_pollset_kick"] = massage_qps_stats_helpers.counter(core_stats, "pollset_kick")
    stats["core_pollset_kicked_without_poller"] = massage_qps_stats_helpers.counter(core_stats, "pollset_kicked_without_poller")
    stats["core_pollset_kicked_again"] = massage_qps_stats_helpers.counter(core_stats, "pollset_kicked_again")
    stats["core_pollset_kick_wakeup_fd"] = massage_qps_stats_helpers.counter(core_stats, "pollset_kick_wakeup_fd")
    stats["core_pollset_kick_wakeup_cv"] = massage_qps_stats_helpers.counter(core_stats, "pollset_kick_wakeup_cv")
    stats["core_pollset_kick_own_thread"] = massage_qps_stats_helpers.counter(core_stats, "pollset_kick_own_thread")
    stats["core_histogram_slow_lookups"] = massage_qps_stats_helpers.counter(core_stats, "histogram_slow_lookups")
    stats["core_syscall_write"] = massage_qps_stats_helpers.counter(core_stats, "syscall_write")
    stats["core_syscall_read"] = massage_qps_stats_helpers.counter(core_stats, "syscall_read")
    stats["core_tcp_backup_pollers_created"] = massage_qps_stats_helpers.counter(core_stats, "tcp_backup_pollers_created")
    stats["core_tcp_backup_poller_polls"] = massage_qps_stats_helpers.counter(core_stats, "tcp_backup_poller_polls")
    stats["core_http2_op_batches"] = massage_qps_stats_helpers.counter(core_stats, "http2_op_batches")
    stats["core_http2_op_cancel"] = massage_qps_stats_helpers.counter(core_stats, "http2_op_cancel")
    stats["core_http2_op_send_initial_metadata"] = massage_qps_stats_helpers.counter(core_stats, "http2_op_send_initial_metadata")
    stats["core_http2_op_send_message"] = massage_qps_stats_helpers.counter(core_stats, "http2_op_send_message")
    stats["core_http2_op_send_trailing_metadata"] = massage_qps_stats_helpers.counter(core_stats, "http2_op_send_trailing_metadata")
    stats["core_http2_op_recv_initial_metadata"] = massage_qps_stats_helpers.counter(core_stats, "http2_op_recv_initial_metadata")
    stats["core_http2_op_recv_message"] = massage_qps_stats_helpers.counter(core_stats, "http2_op_recv_message")
    stats["core_http2_op_recv_trailing_metadata"] = massage_qps_stats_helpers.counter(core_stats, "http2_op_recv_trailing_metadata")
    stats["core_http2_settings_writes"] = massage_qps_stats_helpers.counter(core_stats, "http2_settings_writes")
    stats["core_http2_pings_sent"] = massage_qps_stats_helpers.counter(core_stats, "http2_pings_sent")
    stats["core_http2_writes_begun"] = massage_qps_stats_helpers.counter(core_stats, "http2_writes_begun")
    stats["core_http2_writes_offloaded"] = massage_qps_stats_helpers.counter(core_stats, "http2_writes_offloaded")
    stats["core_http2_writes_continued"] = massage_qps_stats_helpers.counter(core_stats, "http2_writes_continued")
    stats["core_http2_partial_writes"] = massage_qps_stats_helpers.counter(core_stats, "http2_partial_writes")
    stats["core_http2_initiate_write_due_to_initial_write"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_initial_write")
    stats["core_http2_initiate_write_due_to_start_new_stream"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_start_new_stream")
    stats["core_http2_initiate_write_due_to_send_message"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_send_message")
    stats["core_http2_initiate_write_due_to_send_initial_metadata"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_send_initial_metadata")
    stats["core_http2_initiate_write_due_to_send_trailing_metadata"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_send_trailing_metadata")
    stats["core_http2_initiate_write_due_to_retry_send_ping"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_retry_send_ping")
    stats["core_http2_initiate_write_due_to_continue_pings"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_continue_pings")
    stats["core_http2_initiate_write_due_to_goaway_sent"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_goaway_sent")
    stats["core_http2_initiate_write_due_to_rst_stream"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_rst_stream")
    stats["core_http2_initiate_write_due_to_close_from_api"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_close_from_api")
    stats["core_http2_initiate_write_due_to_stream_flow_control"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_stream_flow_control")
    stats["core_http2_initiate_write_due_to_transport_flow_control"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_transport_flow_control")
    stats["core_http2_initiate_write_due_to_send_settings"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_send_settings")
    stats["core_http2_initiate_write_due_to_bdp_estimator_ping"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_bdp_estimator_ping")
    stats["core_http2_initiate_write_due_to_flow_control_unstalled_by_setting"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_flow_control_unstalled_by_setting")
    stats["core_http2_initiate_write_due_to_flow_control_unstalled_by_update"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_flow_control_unstalled_by_update")
    stats["core_http2_initiate_write_due_to_application_ping"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_application_ping")
    stats["core_http2_initiate_write_due_to_keepalive_ping"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_keepalive_ping")
    stats["core_http2_initiate_write_due_to_transport_flow_control_unstalled"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_transport_flow_control_unstalled")
    stats["core_http2_initiate_write_due_to_ping_response"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_ping_response")
    stats["core_http2_initiate_write_due_to_force_rst_stream"] = massage_qps_stats_helpers.counter(core_stats, "http2_initiate_write_due_to_force_rst_stream")
    stats["core_hpack_recv_indexed"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_indexed")
    stats["core_hpack_recv_lithdr_incidx"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_lithdr_incidx")
    stats["core_hpack_recv_lithdr_incidx_v"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_lithdr_incidx_v")
    stats["core_hpack_recv_lithdr_notidx"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_lithdr_notidx")
    stats["core_hpack_recv_lithdr_notidx_v"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_lithdr_notidx_v")
    stats["core_hpack_recv_lithdr_nvridx"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_lithdr_nvridx")
    stats["core_hpack_recv_lithdr_nvridx_v"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_lithdr_nvridx_v")
    stats["core_hpack_recv_uncompressed"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_uncompressed")
    stats["core_hpack_recv_huffman"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_huffman")
    stats["core_hpack_recv_binary"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_binary")
    stats["core_hpack_recv_binary_base64"] = massage_qps_stats_helpers.counter(core_stats, "hpack_recv_binary_base64")
    stats["core_hpack_send_indexed"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_indexed")
    stats["core_hpack_send_lithdr_incidx"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_lithdr_incidx")
    stats["core_hpack_send_lithdr_incidx_v"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_lithdr_incidx_v")
    stats["core_hpack_send_lithdr_notidx"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_lithdr_notidx")
    stats["core_hpack_send_lithdr_notidx_v"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_lithdr_notidx_v")
    stats["core_hpack_send_lithdr_nvridx"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_lithdr_nvridx")
    stats["core_hpack_send_lithdr_nvridx_v"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_lithdr_nvridx_v")
    stats["core_hpack_send_uncompressed"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_uncompressed")
    stats["core_hpack_send_huffman"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_huffman")
    stats["core_hpack_send_binary"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_binary")
    stats["core_hpack_send_binary_base64"] = massage_qps_stats_helpers.counter(core_stats, "hpack_send_binary_base64")
    stats["core_combiner_locks_initiated"] = massage_qps_stats_helpers.counter(core_stats, "combiner_locks_initiated")
    stats["core_combiner_locks_scheduled_items"] = massage_qps_stats_helpers.counter(core_stats, "combiner_locks_scheduled_items")
    stats["core_combiner_locks_scheduled_final_items"] = massage_qps_stats_helpers.counter(core_stats, "combiner_locks_scheduled_final_items")
    stats["core_combiner_locks_offloaded"] = massage_qps_stats_helpers.counter(core_stats, "combiner_locks_offloaded")
    stats["core_executor_scheduled_short_items"] = massage_qps_stats_helpers.counter(core_stats, "executor_scheduled_short_items")
    stats["core_executor_scheduled_long_items"] = massage_qps_stats_helpers.counter(core_stats, "executor_scheduled_long_items")
    stats["core_executor_scheduled_to_self"] = massage_qps_stats_helpers.counter(core_stats, "executor_scheduled_to_self")
    stats["core_executor_wakeup_initiated"] = massage_qps_stats_helpers.counter(core_stats, "executor_wakeup_initiated")
    stats["core_executor_queue_drained"] = massage_qps_stats_helpers.counter(core_stats, "executor_queue_drained")
    stats["core_executor_push_retries"] = massage_qps_stats_helpers.counter(core_stats, "executor_push_retries")
    stats["core_server_requested_calls"] = massage_qps_stats_helpers.counter(core_stats, "server_requested_calls")
    stats["core_server_slowpath_requests_queued"] = massage_qps_stats_helpers.counter(core_stats, "server_slowpath_requests_queued")
    stats["core_completion_queue_event_pop"] = massage_qps_stats_helpers.counter(core_stats, "completion_queue_event_pop")
    stats["core_completion_queue_event_pop_acquired_lock"] = massage_qps_stats_helpers.counter(core_stats, "completion_queue_event_pop_acquired_lock")
    h = massage_qps_stats_helpers.histogram(core_stats, "call_initial_size")
    stats["core_call_initial_size"] = ",".join("%f" % x for x in h.buckets)
    stats["core_call_initial_size_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_call_initial_size_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_call_initial_size_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_call_initial_size_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "poll_events_returned")
    stats["core_poll_events_returned"] = ",".join("%f" % x for x in h.buckets)
    stats["core_poll_events_returned_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_poll_events_returned_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_poll_events_returned_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_poll_events_returned_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "tcp_write_size")
    stats["core_tcp_write_size"] = ",".join("%f" % x for x in h.buckets)
    stats["core_tcp_write_size_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_tcp_write_size_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_tcp_write_size_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_tcp_write_size_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "tcp_write_iov_size")
    stats["core_tcp_write_iov_size"] = ",".join("%f" % x for x in h.buckets)
    stats["core_tcp_write_iov_size_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_tcp_write_iov_size_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_tcp_write_iov_size_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_tcp_write_iov_size_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "tcp_read_size")
    stats["core_tcp_read_size"] = ",".join("%f" % x for x in h.buckets)
    stats["core_tcp_read_size_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_tcp_read_size_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_tcp_read_size_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_tcp_read_size_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "tcp_read_offer")
    stats["core_tcp_read_offer"] = ",".join("%f" % x for x in h.buckets)
    stats["core_tcp_read_offer_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_tcp_read_offer_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_tcp_read_offer_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_tcp_read_offer_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "tcp_read_offer_iov_size")
    stats["core_tcp_read_offer_iov_size"] = ",".join("%f" % x for x in h.buckets)
    stats["core_tcp_read_offer_iov_size_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_tcp_read_offer_iov_size_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_tcp_read_offer_iov_size_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_tcp_read_offer_iov_size_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "http2_send_message_size")
    stats["core_http2_send_message_size"] = ",".join("%f" % x for x in h.buckets)
    stats["core_http2_send_message_size_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_http2_send_message_size_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_http2_send_message_size_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_http2_send_message_size_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "http2_send_initial_metadata_per_write")
    stats["core_http2_send_initial_metadata_per_write"] = ",".join("%f" % x for x in h.buckets)
    stats["core_http2_send_initial_metadata_per_write_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_http2_send_initial_metadata_per_write_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_http2_send_initial_metadata_per_write_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_http2_send_initial_metadata_per_write_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "http2_send_message_per_write")
    stats["core_http2_send_message_per_write"] = ",".join("%f" % x for x in h.buckets)
    stats["core_http2_send_message_per_write_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_http2_send_message_per_write_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_http2_send_message_per_write_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_http2_send_message_per_write_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "http2_send_trailing_metadata_per_write")
    stats["core_http2_send_trailing_metadata_per_write"] = ",".join("%f" % x for x in h.buckets)
    stats["core_http2_send_trailing_metadata_per_write_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_http2_send_trailing_metadata_per_write_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_http2_send_trailing_metadata_per_write_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_http2_send_trailing_metadata_per_write_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "http2_send_flowctl_per_write")
    stats["core_http2_send_flowctl_per_write"] = ",".join("%f" % x for x in h.buckets)
    stats["core_http2_send_flowctl_per_write_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_http2_send_flowctl_per_write_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_http2_send_flowctl_per_write_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_http2_send_flowctl_per_write_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
    h = massage_qps_stats_helpers.histogram(core_stats, "server_cqs_checked")
    stats["core_server_cqs_checked"] = ",".join("%f" % x for x in h.buckets)
    stats["core_server_cqs_checked_bkts"] = ",".join("%f" % x for x in h.boundaries)
    stats["core_server_cqs_checked_50p"] = massage_qps_stats_helpers.percentile(h.buckets, 50, h.boundaries)
    stats["core_server_cqs_checked_95p"] = massage_qps_stats_helpers.percentile(h.buckets, 95, h.boundaries)
    stats["core_server_cqs_checked_99p"] = massage_qps_stats_helpers.percentile(h.buckets, 99, h.boundaries)
