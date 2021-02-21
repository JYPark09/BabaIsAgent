import torch
from torch import nn


class ResBlock(nn.Module):
    def __init__(self, filters: int):
        super(ResBlock, self).__init__()

        self.conv_lower = nn.Sequential(
            nn.Conv2d(filters, filters, 3, padding=1, bias=False),
            nn.BatchNorm2d(filters),
            nn.ReLU(inplace=True)
        )

        self.conv_upper = nn.Sequential(
            nn.Conv2d(filters, filters, 3, padding=1, bias=False),
            nn.BatchNorm2d(filters)
        )

        self.relu = nn.ReLU(inplace=True)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        path = self.conv_lower(x)
        path = self.conv_upper(path)

        return self.relu(x + path)


class ResNet(nn.Module):
    def __init__(self, in_channel: int, filters: int, blocks: int, action_size: int, export=False):
        super(ResNet, self).__init__()

        self.init_conv = nn.Sequential(
            nn.Conv2d(in_channel, filters, 5, padding=2, bias=False),
            nn.BatchNorm2d(filters),
            nn.ReLU(inplace=True)
        )

        self.res_blocks = nn.Sequential(
            *[ResBlock(filters) for _ in range(blocks)]
        )

        self.avg_pool = nn.AdaptiveAvgPool2d(1)

        self.policy = nn.Sequential(
            nn.Flatten(),
            nn.Linear(filters, action_size),
        )
        self.softmax = nn.Softmax(dim=1) if export else nn.LogSoftmax(dim=1)

        self.value = nn.Sequential(
            nn.Flatten(),
            nn.Linear(filters, 256),
            nn.ReLU(inplace=True),
            nn.Linear(256, 1),
            nn.Tanh()
        )

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        x = self.init_conv(x)
        x = self.res_blocks(x)

        x = self.avg_pool(x)

        policy = self.policy(x)
        policy = self.softmax(policy)

        value = self.value(x)

        return policy, value
