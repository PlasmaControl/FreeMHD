
autoPtr<suspensionEntrainmentModel> entrainment
(
    suspensionEntrainmentModel::New(transportProperties, Us, h, hentrain, c, tau)
);

autoPtr<suspensionFrictionModel> friction
(
    suspensionFrictionModel::New(transportProperties, Us, h, c)
);
