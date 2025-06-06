#include <contact_system/contact_reporter.h>

namespace uipc::backend::cuda
{
void ContactReporter::report_extent(GlobalContactManager::ContactExtentInfo& info)
{
    do_report_extent(info);
}
void ContactReporter::assemble(GlobalContactManager::ContactInfo& info)
{
    do_assemble(info);
}
void ContactReporter::compute_energy(GlobalContactManager::EnergyInfo& info)
{
    do_compute_energy(info);
}
}  // namespace uipc::backend::cuda
