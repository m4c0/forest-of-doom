module fox;

static constexpr const auto uber_dset_smps = 8;

static auto create_sampler() {
  auto info = vee::sampler_create_info {};
  info.address_mode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
  info.nearest();
  info.unnormalizedCoordinates = wagen::vk_true;
  return vee::create_sampler(info);
}

fox::uber_dset::uber_dset() :
  m_smp { create_sampler() }
, m_dset {
  vee::dsl_fragment_samplers([this] {
    hai::array<vee::sampler::type> res { uber_dset_smps };
    for (auto & s : res) s = *m_smp;
    return res;
  }()),
  vee::combined_image_sampler(uber_dset_smps),
}
, m_img {}
{
  auto img = sires::real_path_name("1_Terrains_and_Fences_16x16.png");
  auto pd = v::dq()->physical_device();
  auto q = v::dq()->queue();
  voo::load_image(img, pd, q, &m_img, [this] {
    for (auto i = 0; i < uber_dset_smps; i++) {
      vee::update_descriptor_set(descriptor_set(), 0, i, *m_img.iv, *m_smp);
    }
  });
}
